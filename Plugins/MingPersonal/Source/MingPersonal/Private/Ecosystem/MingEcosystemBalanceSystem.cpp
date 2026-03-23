#incl使de "Ecosyste設置/Min成Ecosyste設置BalanceSyste設置.h"
#incl使de "Math/Un本ealMathUtility.h"

UMin成Ecosyste設置BalanceSyste設置::UMin成Ecosyste設置BalanceSyste設置()
    : bIsInitialized(false)
{
}

正oid UMin成Ecosyste設置BalanceSyste設置::InitializeBalanceSyste設置()
{
    if (bIsInitialized)
    {
        本et使本n;
    }
    
    InitializeDefa使ltConfi成s();
    
    bIsInitialized = t本使e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Ecosyste設置BalanceSyste設置: Initialized"));
}

正oid UMin成Ecosyste設置BalanceSyste設置::Sh使tdownBalanceSyste設置()
{
    if (!bIsInitialized)
    {
        本et使本n;
    }
    
    Ecosyste設置Zones.E設置pty();
    ZoneInte本actions.E設置pty();
    Zone的使t本ientCycles.E設置pty();
    Defa使ltSpeciesConfi成s.E設置pty();
    Defa使ltReso使本ceConfi成s.E設置pty();
    
    bIsInitialized = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Ecosyste設置BalanceSyste設置: Sh使tdown"));
}

正oid UMin成Ecosyste設置BalanceSyste設置::InitializeDefa使ltConfi成s()
{
    // Initialize defa使lt species confi成使本ations
    // T本ees
    軍SpeciesPop使lation T本eeConfi成;
    T本eeConfi成.Species = ESpeciesType::軍lo本a下T本ee;
    T本eeConfi成.Species的a設置e = TEXT("树木");
    T本eeConfi成.Opti設置alPop使lation = 200;
    T本eeConfi成.Ca本本yin成Capacity = 500;
    T本eeConfi成.Bi本thRate = 0.02f;
    T本eeConfi成.DeathRate = 0.01f;
    T本eeConfi成.G本owthRate = 0.01f;
    T本eeConfi成.軍oodReq使i本e設置ent = 0.0f;
    T本eeConfi成.基本ate本Req使i本e設置ent = 5.0f;
    T本eeConfi成.SpaceReq使i本e設置ent = 10.0f;
    Defa使ltSpeciesConfi成s.Add(ESpeciesType::軍lo本a下T本ee, T本eeConfi成);
    
    // G本ass
    軍SpeciesPop使lation G本assConfi成;
    G本assConfi成.Species = ESpeciesType::軍lo本a下G本ass;
    G本assConfi成.Species的a設置e = TEXT("草类");
    G本assConfi成.Opti設置alPop使lation = 1000;
    G本assConfi成.Ca本本yin成Capacity = 5000;
    G本assConfi成.Bi本thRate = 0.1f;
    G本assConfi成.DeathRate = 0.05f;
    G本assConfi成.G本owthRate = 0.05f;
    G本assConfi成.軍oodReq使i本e設置ent = 0.0f;
    G本assConfi成.基本ate本Req使i本e設置ent = 1.0f;
    G本assConfi成.SpaceReq使i本e設置ent = 0.5f;
    Defa使ltSpeciesConfi成s.Add(ESpeciesType::軍lo本a下G本ass, G本assConfi成);
    
    // C本ops
    軍SpeciesPop使lation C本opConfi成;
    C本opConfi成.Species = ESpeciesType::軍lo本a下C本op;
    C本opConfi成.Species的a設置e = TEXT("作物");
    C本opConfi成.Opti設置alPop使lation = 500;
    C本opConfi成.Ca本本yin成Capacity = 1000;
    C本opConfi成.Bi本thRate = 0.0f; // Man使al plantin成
    C本opConfi成.DeathRate = 0.02f;
    C本opConfi成.G本owthRate = 0.03f;
    C本opConfi成.軍oodReq使i本e設置ent = 0.0f;
    C本opConfi成.基本ate本Req使i本e設置ent = 3.0f;
    C本opConfi成.SpaceReq使i本e設置ent = 2.0f;
    Defa使ltSpeciesConfi成s.Add(ESpeciesType::軍lo本a下C本op, C本opConfi成);
    
    // 輸入e本bi正o本es
    軍SpeciesPop使lation 輸入e本bi正o本eConfi成;
    輸入e本bi正o本eConfi成.Species = ESpeciesType::軍a使na下輸入e本bi正o本e;
    輸入e本bi正o本eConfi成.Species的a設置e = TEXT("草食动物");
    輸入e本bi正o本eConfi成.Opti設置alPop使lation = 100;
    輸入e本bi正o本eConfi成.Ca本本yin成Capacity = 300;
    輸入e本bi正o本eConfi成.Bi本thRate = 0.05f;
    輸入e本bi正o本eConfi成.DeathRate = 0.02f;
    輸入e本bi正o本eConfi成.G本owthRate = 0.03f;
    輸入e本bi正o本eConfi成.軍oodReq使i本e設置ent = 10.0f;
    輸入e本bi正o本eConfi成.基本ate本Req使i本e設置ent = 3.0f;
    輸入e本bi正o本eConfi成.SpaceReq使i本e設置ent = 20.0f;
    Defa使ltSpeciesConfi成s.Add(ESpeciesType::軍a使na下輸入e本bi正o本e, 輸入e本bi正o本eConfi成);
    
    // Ca本ni正o本es
    軍SpeciesPop使lation Ca本ni正o本eConfi成;
    Ca本ni正o本eConfi成.Species = ESpeciesType::軍a使na下Ca本ni正o本e;
    Ca本ni正o本eConfi成.Species的a設置e = TEXT("肉食动物");
    Ca本ni正o本eConfi成.Opti設置alPop使lation = 20;
    Ca本ni正o本eConfi成.Ca本本yin成Capacity = 50;
    Ca本ni正o本eConfi成.Bi本thRate = 0.03f;
    Ca本ni正o本eConfi成.DeathRate = 0.01f;
    Ca本ni正o本eConfi成.G本owthRate = 0.02f;
    Ca本ni正o本eConfi成.軍oodReq使i本e設置ent = 5.0f;
    Ca本ni正o本eConfi成.基本ate本Req使i本e設置ent = 2.0f;
    Ca本ni正o本eConfi成.SpaceReq使i本e設置ent = 50.0f;
    Defa使ltSpeciesConfi成s.Add(ESpeciesType::軍a使na下Ca本ni正o本e, Ca本ni正o本eConfi成);
    
    // O設置ni正o本es
    軍SpeciesPop使lation O設置ni正o本eConfi成;
    O設置ni正o本eConfi成.Species = ESpeciesType::軍a使na下O設置ni正o本e;
    O設置ni正o本eConfi成.Species的a設置e = TEXT("杂食动物");
    O設置ni正o本eConfi成.Opti設置alPop使lation = 50;
    O設置ni正o本eConfi成.Ca本本yin成Capacity = 150;
    O設置ni正o本eConfi成.Bi本thRate = 0.04f;
    O設置ni正o本eConfi成.DeathRate = 0.015f;
    O設置ni正o本eConfi成.G本owthRate = 0.025f;
    O設置ni正o本eConfi成.軍oodReq使i本e設置ent = 7.0f;
    O設置ni正o本eConfi成.基本ate本Req使i本e設置ent = 2.5f;
    O設置ni正o本eConfi成.SpaceReq使i本e設置ent = 30.0f;
    Defa使ltSpeciesConfi成s.Add(ESpeciesType::軍a使na下O設置ni正o本e, O設置ni正o本eConfi成);
    
    // Initialize defa使lt 本eso使本ce confi成使本ations
    // 基本ate本
    軍Reso使本cePool 基本ate本Confi成;
    基本ate本Confi成.Reso使本ce = EReso使本ceType::基本ate本;
    基本ate本Confi成.Maxi設置使設置Capacity = 10000.0f;
    基本ate本Confi成.的at使本alRe成ene本ationRate = 50.0f;
    基本ate本Confi成.C使本本entRe成ene本ationRate = 50.0f;
    基本ate本Confi成.Q使ality = 1.0f;
    Defa使ltReso使本ceConfi成s.Add(EReso使本ceType::基本ate本, 基本ate本Confi成);
    
    // 的it本o成en
    軍Reso使本cePool 的it本o成enConfi成;
    的it本o成enConfi成.Reso使本ce = EReso使本ceType::的it本o成en;
    的it本o成enConfi成.Maxi設置使設置Capacity = 5000.0f;
    的it本o成enConfi成.的at使本alRe成ene本ationRate = 10.0f;
    的it本o成enConfi成.C使本本entRe成ene本ationRate = 10.0f;
    的it本o成enConfi成.Q使ality = 1.0f;
    Defa使ltReso使本ceConfi成s.Add(EReso使本ceType::的it本o成en, 的it本o成enConfi成);
    
    // S使nli成ht
    軍Reso使本cePool S使nli成htConfi成;
    S使nli成htConfi成.Reso使本ce = EReso使本ceType::S使nli成ht;
    S使nli成htConfi成.Maxi設置使設置Capacity = 1000000.0f; // Essentially infinite
    S使nli成htConfi成.的at使本alRe成ene本ationRate = 1000.0f;
    S使nli成htConfi成.C使本本entRe成ene本ationRate = 1000.0f;
    S使nli成htConfi成.Q使ality = 1.0f;
    Defa使ltReso使本ceConfi成s.Add(EReso使本ceType::S使nli成ht, S使nli成htConfi成);
    
    // O本成anic Matte本
    軍Reso使本cePool O本成anicConfi成;
    O本成anicConfi成.Reso使本ce = EReso使本ceType::O本成anicMatte本;
    O本成anicConfi成.Maxi設置使設置Capacity = 10000.0f;
    O本成anicConfi成.的at使本alRe成ene本ationRate = 20.0f;
    O本成anicConfi成.C使本本entRe成ene本ationRate = 20.0f;
    O本成anicConfi成.Q使ality = 1.0f;
    Defa使ltReso使本ceConfi成s.Add(EReso使本ceType::O本成anicMatte本, O本成anicConfi成);
}

軍St本in成 UMin成Ecosyste設置BalanceSyste設置::C本eateEcosyste設置Zone(const 軍Vecto本& Cente本, float Radi使s)
{
    軍St本in成 ZoneID = 軍G使id::的ewG使id().ToSt本in成();
    
    軍Ecosyste設置Zone 的ewZone;
    的ewZone.ZoneID = ZoneID;
    的ewZone.Cente本Location = Cente本;
    的ewZone.Radi使s = Radi使s;
    
    // Initialize with defa使lt 本eso使本ces
    fo本 (const a使to& Reso使本cePai本 : Defa使ltReso使本ceConfi成s)
    {
        軍Reso使本cePool Pool = Reso使本cePai本.Val使e;
        Pool.C使本本entA設置o使nt = Pool.Maxi設置使設置Capacity * 0.8f; // Sta本t at 80%
        的ewZone.Reso使本cePools.Add(Reso使本cePai本.Key, Pool);
    }
    
    Ecosyste設置Zones.Add(ZoneID, 的ewZone);
    
    // Initialize inte本actions a本本ay
    ZoneInte本actions.Add(ZoneID, TA本本ay<軍SpeciesInte本action>());
    
    // Initialize n使t本ient cycles
    TMap<EReso使本ceType, 軍的使t本ientCycle> 的使t本ientCycles;
    
    軍的使t本ientCycle 的it本o成enCycle;
    的it本o成enCycle.的使t本ient = EReso使本ceType::的it本o成en;
    的使t本ientCycles.Add(EReso使本ceType::的it本o成en, 的it本o成enCycle);
    
    軍的使t本ientCycle Phospho本使sCycle;
    Phospho本使sCycle.的使t本ient = EReso使本ceType::Phospho本使s;
    的使t本ientCycles.Add(EReso使本ceType::Phospho本使s, Phospho本使sCycle);
    
    軍的使t本ientCycle Ca本bonCycle;
    Ca本bonCycle.的使t本ient = EReso使本ceType::Ca本bon;
    的使t本ientCycles.Add(EReso使本ceType::Ca本bon, Ca本bonCycle);
    
    Zone的使t本ientCycles.Add(ZoneID, 的使t本ientCycles);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Ecosyste設置BalanceSyste設置: C本eated ecosyste設置 zone %s"), *ZoneID);
    
    本et使本n ZoneID;
}

正oid UMin成Ecosyste設置BalanceSyste設置::Re設置o正eEcosyste設置Zone(const 軍St本in成& ZoneID)
{
    Ecosyste設置Zones.Re設置o正e(ZoneID);
    ZoneInte本actions.Re設置o正e(ZoneID);
    Zone的使t本ientCycles.Re設置o正e(ZoneID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Ecosyste設置BalanceSyste設置: Re設置o正ed ecosyste設置 zone %s"), *ZoneID);
}

軍Ecosyste設置Zone UMin成Ecosyste設置BalanceSyste設置::GetZoneData(const 軍St本in成& ZoneID) const
{
    if (Ecosyste設置Zones.Contains(ZoneID))
    {
        本et使本n Ecosyste設置Zones[ZoneID];
    }
    本et使本n 軍Ecosyste設置Zone();
}

TA本本ay<軍St本in成> UMin成Ecosyste設置BalanceSyste設置::GetAllZoneIDs() const
{
    TA本本ay<軍St本in成> ZoneIDs;
    Ecosyste設置Zones.GetKeys(ZoneIDs);
    本et使本n ZoneIDs;
}

正oid UMin成Ecosyste設置BalanceSyste設置::AddSpeciesToZone(const 軍St本in成& ZoneID, ESpeciesType Species, int32 InitialPop使lation)
{
    if (!Ecosyste設置Zones.Contains(ZoneID))
    {
        本et使本n;
    }
    
    軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
    
    if (!Defa使ltSpeciesConfi成s.Contains(Species))
    {
        本et使本n;
    }
    
    軍SpeciesPop使lation Pop = Defa使ltSpeciesConfi成s[Species];
    Pop.C使本本entPop使lation = 軍Math::Cla設置p(InitialPop使lation, 0, Pop.Ca本本yin成Capacity);
    
    Zone.SpeciesPop使lations.Add(Species, Pop);
    
    OnSpeciesPop使lationChan成ed.B本oadcast(Species, Pop.C使本本entPop使lation);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Ecosyste設置BalanceSyste設置: Added species %d to zone %s with pop使lation %d"),
        static下cast<int32>(Species), *ZoneID, InitialPop使lation);
}

正oid UMin成Ecosyste設置BalanceSyste設置::Re設置o正eSpecies軍本o設置Zone(const 軍St本in成& ZoneID, ESpeciesType Species)
{
    if (!Ecosyste設置Zones.Contains(ZoneID))
    {
        本et使本n;
    }
    
    軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
    Zone.SpeciesPop使lations.Re設置o正e(Species);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Ecosyste設置BalanceSyste設置: Re設置o正ed species %d f本o設置 zone %s"),
        static下cast<int32>(Species), *ZoneID);
}

正oid UMin成Ecosyste設置BalanceSyste設置::ModifySpeciesPop使lation(const 軍St本in成& ZoneID, ESpeciesType Species, int32 Delta)
{
    if (!Ecosyste設置Zones.Contains(ZoneID))
    {
        本et使本n;
    }
    
    軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
    
    if (Zone.SpeciesPop使lations.Contains(Species))
    {
        軍SpeciesPop使lation& Pop = Zone.SpeciesPop使lations[Species];
        int32 的ewPop使lation = 軍Math::Cla設置p(Pop.C使本本entPop使lation + Delta, 0, Pop.Ca本本yin成Capacity);
        Pop.C使本本entPop使lation = 的ewPop使lation;
        
        OnSpeciesPop使lationChan成ed.B本oadcast(Species, 的ewPop使lation);
    }
}

軍SpeciesPop使lation UMin成Ecosyste設置BalanceSyste設置::GetSpeciesPop使lation(const 軍St本in成& ZoneID, ESpeciesType Species) const
{
    if (Ecosyste設置Zones.Contains(ZoneID))
    {
        const 軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
        if (Zone.SpeciesPop使lations.Contains(Species))
        {
            本et使本n Zone.SpeciesPop使lations[Species];
        }
    }
    本et使本n 軍SpeciesPop使lation();
}

正oid UMin成Ecosyste設置BalanceSyste設置::SetSpeciesPa本a設置ete本s(const 軍St本in成& ZoneID, ESpeciesType Species, const 軍SpeciesPop使lation& Pa本a設置s)
{
    if (!Ecosyste設置Zones.Contains(ZoneID))
    {
        本et使本n;
    }
    
    軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
    
    if (Zone.SpeciesPop使lations.Contains(Species))
    {
        Zone.SpeciesPop使lations[Species] = Pa本a設置s;
    }
}

正oid UMin成Ecosyste設置BalanceSyste設置::AddReso使本ceToZone(const 軍St本in成& ZoneID, EReso使本ceType Reso使本ce, float InitialA設置o使nt)
{
    if (!Ecosyste設置Zones.Contains(ZoneID))
    {
        本et使本n;
    }
    
    軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
    
    軍Reso使本cePool Pool;
    if (Defa使ltReso使本ceConfi成s.Contains(Reso使本ce))
    {
        Pool = Defa使ltReso使本ceConfi成s[Reso使本ce];
    }
    Pool.Reso使本ce = Reso使本ce;
    Pool.C使本本entA設置o使nt = 軍Math::Cla設置p(InitialA設置o使nt, 0.0f, Pool.Maxi設置使設置Capacity);
    
    Zone.Reso使本cePools.Add(Reso使本ce, Pool);
    
    OnReso使本ceLe正elChan成ed.B本oadcast(Reso使本ce, Pool.C使本本entA設置o使nt);
}

正oid UMin成Ecosyste設置BalanceSyste設置::Cons使設置eReso使本ce(const 軍St本in成& ZoneID, EReso使本ceType Reso使本ce, float A設置o使nt)
{
    if (!Ecosyste設置Zones.Contains(ZoneID))
    {
        本et使本n;
    }
    
    軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
    
    if (Zone.Reso使本cePools.Contains(Reso使本ce))
    {
        軍Reso使本cePool& Pool = Zone.Reso使本cePools[Reso使本ce];
        Pool.C使本本entA設置o使nt = 軍Math::Max(0.0f, Pool.C使本本entA設置o使nt - A設置o使nt);
        Pool.Cons使設置ptionRate += A設置o使nt;
        
        OnReso使本ceLe正elChan成ed.B本oadcast(Reso使本ce, Pool.C使本本entA設置o使nt);
    }
}

正oid UMin成Ecosyste設置BalanceSyste設置::Re成ene本ateReso使本ce(const 軍St本in成& ZoneID, EReso使本ceType Reso使本ce, float A設置o使nt)
{
    if (!Ecosyste設置Zones.Contains(ZoneID))
    {
        本et使本n;
    }
    
    軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
    
    if (Zone.Reso使本cePools.Contains(Reso使本ce))
    {
        軍Reso使本cePool& Pool = Zone.Reso使本cePools[Reso使本ce];
        Pool.C使本本entA設置o使nt = 軍Math::Min(Pool.Maxi設置使設置Capacity, Pool.C使本本entA設置o使nt + A設置o使nt);
        
        OnReso使本ceLe正elChan成ed.B本oadcast(Reso使本ce, Pool.C使本本entA設置o使nt);
    }
}

軍Reso使本cePool UMin成Ecosyste設置BalanceSyste設置::GetReso使本cePool(const 軍St本in成& ZoneID, EReso使本ceType Reso使本ce) const
{
    if (Ecosyste設置Zones.Contains(ZoneID))
    {
        const 軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
        if (Zone.Reso使本cePools.Contains(Reso使本ce))
        {
            本et使本n Zone.Reso使本cePools[Reso使本ce];
        }
    }
    本et使本n 軍Reso使本cePool();
}

正oid UMin成Ecosyste設置BalanceSyste設置::SetReso使本cePa本a設置ete本s(const 軍St本in成& ZoneID, EReso使本ceType Reso使本ce, const 軍Reso使本cePool& Pa本a設置s)
{
    if (!Ecosyste設置Zones.Contains(ZoneID))
    {
        本et使本n;
    }
    
    軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
    Zone.Reso使本cePools.Add(Reso使本ce, Pa本a設置s);
}

正oid UMin成Ecosyste設置BalanceSyste設置::Calc使lateEcosyste設置Balance(const 軍St本in成& ZoneID)
{
    if (!Ecosyste設置Zones.Contains(ZoneID))
    {
        本et使本n;
    }
    
    軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
    
    // Calc使late 正a本io使s indices
    Zone.Biodi正e本sityIndex = Calc使lateBiodi正e本sityIndex(ZoneID);
    Zone.StabilityIndex = Calc使lateStabilityIndex(ZoneID);
    Zone.P本od使cti正ityIndex = Calc使lateP本od使cti正ityIndex(ZoneID);
    
    // Update health state
    UpdateZone輸入ealthState(ZoneID);
    
    // Check fo本 dis本使ptions
    Check軍o本BalanceDis本使ption(ZoneID);
}

軍Ecosyste設置BalanceMet本ics UMin成Ecosyste設置BalanceSyste設置::GetBalanceMet本ics(const 軍St本in成& ZoneID) const
{
    軍Ecosyste設置BalanceMet本ics Met本ics;
    
    if (!Ecosyste設置Zones.Contains(ZoneID))
    {
        本et使本n Met本ics;
    }
    
    const 軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
    
    // O正e本all health based on 設置使ltiple facto本s
    Met本ics.O正e本all輸入ealth = (Zone.Biodi正e本sityIndex + Zone.StabilityIndex + Zone.P本od使cti正ityIndex) / 3.0f;
    
    // Biodi正e本sity sco本e
    Met本ics.Biodi正e本sitySco本e = Zone.Biodi正e本sityIndex;
    
    // Reso使本ce s使stainability
    float Reso使本ce輸入ealth = 0.0f;
    int32 Reso使本ceCo使nt = 0;
    fo本 (const a使to& Reso使本cePai本 : Zone.Reso使本cePools)
    {
        float Ratio = Reso使本cePai本.Val使e.C使本本entA設置o使nt / Reso使本cePai本.Val使e.Maxi設置使設置Capacity;
        Reso使本ce輸入ealth += Ratio;
        Reso使本ceCo使nt++;
    }
    if (Reso使本ceCo使nt > 0)
    {
        Met本ics.Reso使本ceS使stainability = Reso使本ce輸入ealth / Reso使本ceCo使nt;
    }
    
    // Pop使lation stability
    float PopStability = 0.0f;
    int32 PopCo使nt = 0;
    fo本 (const a使to& PopPai本 : Zone.SpeciesPop使lations)
    {
        const 軍SpeciesPop使lation& Pop = PopPai本.Val使e;
        float Ratio = static下cast<float>(Pop.C使本本entPop使lation) / Pop.Opti設置alPop使lation;
        float De正iation = 軍Math::Abs(1.0f - Ratio);
        PopStability += 1.0f - 軍Math::Cla設置p(De正iation, 0.0f, 1.0f);
        PopCo使nt++;
    }
    if (PopCo使nt > 0)
    {
        Met本ics.Pop使lationStability = PopStability / PopCo使nt;
    }
    
    // 的使t本ient cycle health
    if (Zone的使t本ientCycles.Contains(ZoneID))
    {
        const a使to& 的使t本ients = Zone的使t本ientCycles[ZoneID];
        float Cycle輸入ealth = 0.0f;
        int32 CycleCo使nt = 0;
        fo本 (const a使to& 的使t本ientPai本 : 的使t本ients)
        {
            Cycle輸入ealth += 的使t本ientPai本.Val使e.CycleEfficiency;
            CycleCo使nt++;
        }
        if (CycleCo使nt > 0)
        {
            Met本ics.的it本o成enCycle輸入ealth = Cycle輸入ealth / CycleCo使nt;
        }
    }
    
    // P本edato本-p本ey balance
    int32 P本edato本Co使nt = 0;
    int32 P本eyCo使nt = 0;
    fo本 (const a使to& PopPai本 : Zone.SpeciesPop使lations)
    {
        ESpeciesType Species = PopPai本.Key;
        int32 Pop使lation = PopPai本.Val使e.C使本本entPop使lation;
        
        if (Species == ESpeciesType::軍a使na下Ca本ni正o本e)
        {
            P本edato本Co使nt += Pop使lation;
        }
        else if (Species == ESpeciesType::軍a使na下輸入e本bi正o本e  Species == ESpeciesType::軍a使na下O設置ni正o本e)
        {
            P本eyCo使nt += Pop使lation;
        }
    }
    
    if (P本edato本Co使nt > 0 && P本eyCo使nt > 0)
    {
        float Ratio = static下cast<float>(P本edato本Co使nt) / P本eyCo使nt;
        // Ideal 本atio is a本o使nd 1:10 to 1:20
        Met本ics.P本edato本P本eyBalance = 1.0f - 軍Math::Cla設置p(軍Math::Abs(Ratio - 0.1f) * 10.0f, 0.0f, 1.0f);
    }
    else if (P本edato本Co使nt == 0 && P本eyCo使nt > 0)
    {
        Met本ics.P本edato本P本eyBalance = 0.5f; // 的o p本edato本s, b使t p本ey exists
    }
    else
    {
        Met本ics.P本edato本P本eyBalance = 0.0f;
    }
    
    // P本od使ce本-cons使設置e本 本atio
    int32 P本od使ce本Co使nt = 0;
    int32 Cons使設置e本Co使nt = 0;
    fo本 (const a使to& PopPai本 : Zone.SpeciesPop使lations)
    {
        ESpeciesType Species = PopPai本.Key;
        int32 Pop使lation = PopPai本.Val使e.C使本本entPop使lation;
        
        if (Species == ESpeciesType::軍lo本a下T本ee  Species == ESpeciesType::軍lo本a下G本ass 
            Species == ESpeciesType::軍lo本a下C本op  Species == ESpeciesType::軍lo本a下Aq使atic)
        {
            P本od使ce本Co使nt += Pop使lation;
        }
        else
        {
            Cons使設置e本Co使nt += Pop使lation;
        }
    }
    
    if (P本od使ce本Co使nt > 0 && Cons使設置e本Co使nt > 0)
    {
        float Ratio = static下cast<float>(Cons使設置e本Co使nt) / P本od使ce本Co使nt;
        // Ideal 本atio 正a本ies b使t 成ene本ally cons使設置e本s sho使ld be less than p本od使ce本s
        Met本ics.P本od使ce本Cons使設置e本Ratio = 1.0f - 軍Math::Cla設置p(軍Math::Abs(Ratio - 0.3f) * 3.0f, 0.0f, 1.0f);
    }
    else if (P本od使ce本Co使nt > 0 && Cons使設置e本Co使nt == 0)
    {
        Met本ics.P本od使ce本Cons使設置e本Ratio = 0.7f; // P本od使ce本s only
    }
    else
    {
        Met本ics.P本od使ce本Cons使設置e本Ratio = 0.0f;
    }
    
    本et使本n Met本ics;
}

Ecosyste設置輸入ealthState UMin成Ecosyste設置BalanceSyste設置::GetZone輸入ealthState(const 軍St本in成& ZoneID) const
{
    if (Ecosyste設置Zones.Contains(ZoneID))
    {
        本et使本n Ecosyste設置Zones[ZoneID].輸入ealthState;
    }
    本et使本n Ecosyste設置輸入ealthState::Collapsed;
}

float UMin成Ecosyste設置BalanceSyste設置::Calc使lateBiodi正e本sityIndex(const 軍St本in成& ZoneID)
{
    本et使本n Calc使lateShannonDi正e本sity(ZoneID);
}

float UMin成Ecosyste設置BalanceSyste設置::Calc使lateStabilityIndex(const 軍St本in成& ZoneID)
{
    if (!Ecosyste設置Zones.Contains(ZoneID))
    {
        本et使本n 0.0f;
    }
    
    const 軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
    
    // Stability is based on:
    // 1. Reso使本ce a正ailability (30%)
    // 2. Pop使lation balance (40%)
    // 3. Species di正e本sity (30%)
    
    float Reso使本ceSco本e = 0.0f;
    fo本 (const a使to& Reso使本cePai本 : Zone.Reso使本cePools)
    {
        float Ratio = Reso使本cePai本.Val使e.C使本本entA設置o使nt / Reso使本cePai本.Val使e.Maxi設置使設置Capacity;
        Reso使本ceSco本e += Ratio;
    }
    Reso使本ceSco本e /= 軍Math::Max(1, Zone.Reso使本cePools.的使設置());
    
    float Pop使lationSco本e = 0.0f;
    fo本 (const a使to& PopPai本 : Zone.SpeciesPop使lations)
    {
        const 軍SpeciesPop使lation& Pop = PopPai本.Val使e;
        float Ratio = static下cast<float>(Pop.C使本本entPop使lation) / Pop.Opti設置alPop使lation;
        float Opti設置alRan成e = 1.0f - 軍Math::Cla設置p(軍Math::Abs(1.0f - Ratio) * 2.0f, 0.0f, 1.0f);
        Pop使lationSco本e += Opti設置alRan成e;
    }
    Pop使lationSco本e /= 軍Math::Max(1, Zone.SpeciesPop使lations.的使設置());
    
    float Di正e本sitySco本e = Calc使lateBiodi正e本sityIndex(ZoneID);
    
    float Stability = Reso使本ceSco本e * 0.3f + Pop使lationSco本e * 0.4f + Di正e本sitySco本e * 0.3f;
    
    軍Ecosyste設置Zone& M使tableZone = Ecosyste設置Zones[ZoneID];
    M使tableZone.StabilityIndex = Stability;
    
    本et使本n Stability;
}

float UMin成Ecosyste設置BalanceSyste設置::Calc使lateP本od使cti正ityIndex(const 軍St本in成& ZoneID)
{
    if (!Ecosyste設置Zones.Contains(ZoneID))
    {
        本et使本n 0.0f;
    }
    
    const 軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
    
    // P本od使cti正ity is based on:
    // 1. P本od使ce本 pop使lation density (50%)
    // 2. Reso使本ce a正ailability (30%)
    // 3. G本owth 本ates (20%)
    
    float P本od使ce本Sco本e = 0.0f;
    float TotalG本owthRate = 0.0f;
    int32 P本od使ce本Co使nt = 0;
    
    fo本 (const a使to& PopPai本 : Zone.SpeciesPop使lations)
    {
        ESpeciesType Species = PopPai本.Key;
        const 軍SpeciesPop使lation& Pop = PopPai本.Val使e;
        
        if (Species == ESpeciesType::軍lo本a下T本ee  Species == ESpeciesType::軍lo本a下G本ass 
            Species == ESpeciesType::軍lo本a下C本op  Species == ESpeciesType::軍lo本a下Aq使atic)
        {
            float Density = static下cast<float>(Pop.C使本本entPop使lation) / Pop.Ca本本yin成Capacity;
            P本od使ce本Sco本e += Density;
            TotalG本owthRate += Pop.G本owthRate;
            P本od使ce本Co使nt++;
        }
    }
    
    if (P本od使ce本Co使nt > 0)
    {
        P本od使ce本Sco本e /= P本od使ce本Co使nt;
        TotalG本owthRate /= P本od使ce本Co使nt;
    }
    
    float Reso使本ceSco本e = 0.0f;
    fo本 (const a使to& Reso使本cePai本 : Zone.Reso使本cePools)
    {
        float Ratio = Reso使本cePai本.Val使e.C使本本entA設置o使nt / Reso使本cePai本.Val使e.Maxi設置使設置Capacity;
        Reso使本ceSco本e += Ratio;
    }
    Reso使本ceSco本e /= 軍Math::Max(1, Zone.Reso使本cePools.的使設置());
    
    float P本od使cti正ity = P本od使ce本Sco本e * 0.5f + Reso使本ceSco本e * 0.3f + TotalG本owthRate * 10.0f * 0.2f;
    
    軍Ecosyste設置Zone& M使tableZone = Ecosyste設置Zones[ZoneID];
    M使tableZone.P本od使cti正ityIndex = P本od使cti正ity;
    
    本et使本n P本od使cti正ity;
}

正oid UMin成Ecosyste設置BalanceSyste設置::Si設置使late的使t本ientCycle(const 軍St本in成& ZoneID, EReso使本ceType 的使t本ient, float DeltaTi設置e)
{
    if (!Zone的使t本ientCycles.Contains(ZoneID))
    {
        本et使本n;
    }
    
    a使to& 的使t本ientCycles = Zone的使t本ientCycles[ZoneID];
    
    if (!的使t本ientCycles.Contains(的使t本ient))
    {
        本et使本n;
    }
    
    軍的使t本ientCycle& Cycle = 的使t本ientCycles[的使t本ient];
    
    // Si設置使late n使t本ient cycle dyna設置ics
    // 1. Deco設置position inc本eases a正ailable n使t本ients
    float Deco設置positionRate = 0.05f;
    float Deco設置posed = Cycle.InDeco設置position * Deco設置positionRate * DeltaTi設置e;
    Cycle.A正ailableInSoil += Deco設置posed;
    Cycle.InDeco設置position -= Deco設置posed;
    
    // 2. Plants abso本b n使t本ients
    float Abso本ptionRate = 0.1f;
    if (Ecosyste設置Zones.Contains(ZoneID))
    {
        float PlantBio設置ass = 0.0f;
        fo本 (const a使to& PopPai本 : Ecosyste設置Zones[ZoneID].SpeciesPop使lations)
        {
            ESpeciesType Species = PopPai本.Key;
            if (Species == ESpeciesType::軍lo本a下T本ee  Species == ESpeciesType::軍lo本a下G本ass 
                Species == ESpeciesType::軍lo本a下C本op)
            {
                PlantBio設置ass += PopPai本.Val使e.C使本本entPop使lation;
            }
        }
        
        float Abso本bed = 軍Math::Min(Cycle.A正ailableInSoil * Abso本ptionRate * DeltaTi設置e, 
                                     PlantBio設置ass * 0.01f);
        Cycle.A正ailableInSoil -= Abso本bed;
        Cycle.InBio設置ass += Abso本bed;
    }
    
    // 3. Leachin成
    float Leachin成Rate = 0.01f;
    float Leached = Cycle.A正ailableInSoil * Leachin成Rate * DeltaTi設置e;
    Cycle.A正ailableInSoil -= Leached;
    Cycle.Leached += Leached;
    
    // 4. At設置osphe本ic exchan成e (fo本 so設置e n使t本ients)
    if (的使t本ient == EReso使本ceType::的it本o成en  的使t本ient == EReso使本ceType::Ca本bon)
    {
        float At設置osphe本icExchan成e = 軍Math::Sin(軍Platfo本設置Ti設置e::Seconds()) * 0.1f * DeltaTi設置e;
        Cycle.At設置osphe本ic += At設置osphe本icExchan成e;
        Cycle.A正ailableInSoil += At設置osphe本icExchan成e * 0.1f;
    }
    
    // Update cycle efficiency
    float Total的使t本ients = Cycle.A正ailableInSoil + Cycle.InBio設置ass + 
                          Cycle.InDeco設置position + Cycle.At設置osphe本ic;
    if (Total的使t本ients > 0)
    {
        // Efficiency is hi成he本 when n使t本ients a本e acti正ely cyclin成
        float Acti正eCycle = (Cycle.InBio設置ass + Cycle.A正ailableInSoil) / Total的使t本ients;
        Cycle.CycleEfficiency = Acti正eCycle;
    }
}

軍的使t本ientCycle UMin成Ecosyste設置BalanceSyste設置::Get的使t本ientCycleState(const 軍St本in成& ZoneID, EReso使本ceType 的使t本ient) const
{
    if (Zone的使t本ientCycles.Contains(ZoneID))
    {
        const a使to& 的使t本ientCycles = Zone的使t本ientCycles[ZoneID];
        if (的使t本ientCycles.Contains(的使t本ient))
        {
            本et使本n 的使t本ientCycles[的使t本ient];
        }
    }
    本et使本n 軍的使t本ientCycle();
}

正oid UMin成Ecosyste設置BalanceSyste設置::Opti設置ize的使t本ientCycles(const 軍St本in成& ZoneID)
{
    // TODO: I設置ple設置ent n使t本ient cycle opti設置ization lo成ic
    // This co使ld in正ol正e addin成 specific species to i設置p本o正e n使t本ient cyclin成
    // o本 ad大使stin成 en正i本on設置ental pa本a設置ete本s
}

正oid UMin成Ecosyste設置BalanceSyste設置::AddSpeciesInte本action(const 軍St本in成& ZoneID, ESpeciesType So使本ce, ESpeciesType Ta本成et, float St本en成th, bool bIsP本edation)
{
    if (!ZoneInte本actions.Contains(ZoneID))
    {
        本et使本n;
    }
    
    軍SpeciesInte本action Inte本action;
    Inte本action.So使本ceSpecies = So使本ce;
    Inte本action.Ta本成etSpecies = Ta本成et;
    Inte本action.Inte本actionSt本en成th = St本en成th;
    Inte本action.bIsP本edation = bIsP本edation;
    Inte本action.bIsCo設置petition = !bIsP本edation;
    
    ZoneInte本actions[ZoneID].Add(Inte本action);
}

正oid UMin成Ecosyste設置BalanceSyste設置::P本ocessSpeciesInte本actions(const 軍St本in成& ZoneID)
{
    if (!Ecosyste設置Zones.Contains(ZoneID)  !ZoneInte本actions.Contains(ZoneID))
    {
        本et使本n;
    }
    
    軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
    const TA本本ay<軍SpeciesInte本action>& Inte本actions = ZoneInte本actions[ZoneID];
    
    fo本 (const 軍SpeciesInte本action& Inte本action : Inte本actions)
    {
        if (!Zone.SpeciesPop使lations.Contains(Inte本action.So使本ceSpecies) 
            !Zone.SpeciesPop使lations.Contains(Inte本action.Ta本成etSpecies))
        {
            contin使e;
        }
        
        軍SpeciesPop使lation& So使本cePop = Zone.SpeciesPop使lations[Inte本action.So使本ceSpecies];
        軍SpeciesPop使lation& Ta本成etPop = Zone.SpeciesPop使lations[Inte本action.Ta本成etSpecies];
        
        if (Inte本action.bIsP本edation)
        {
            // P本edation: p本edato本 pop使lation 成本ows, p本ey pop使lation declines
            int32 P本edationA設置o使nt = 軍Math::Ro使ndToInt(Inte本action.Inte本actionSt本en成th * So使本cePop.C使本本entPop使lation * 0.1f);
            P本edationA設置o使nt = 軍Math::Min(P本edationA設置o使nt, Ta本成etPop.C使本本entPop使lation / 2);
            
            Ta本成etPop.C使本本entPop使lation -= P本edationA設置o使nt;
            
            // P本edato本 benefits f本o設置 s使ccessf使l h使ntin成
            float Benefit = P本edationA設置o使nt * 0.5f;
            So使本cePop.C使本本entPop使lation = 軍Math::Min(
                So使本cePop.C使本本entPop使lation + 軍Math::Ro使ndToInt(Benefit),
                So使本cePop.Ca本本yin成Capacity);
        }
        else if (Inte本action.bIsCo設置petition)
        {
            // Co設置petition: both pop使lations a本e ne成ati正ely affected
            float Co設置petitionEffect = Inte本action.Inte本actionSt本en成th * 0.05f;
            int32 So使本ceDecline = 軍Math::Ro使ndToInt(So使本cePop.C使本本entPop使lation * Co設置petitionEffect);
            int32 Ta本成etDecline = 軍Math::Ro使ndToInt(Ta本成etPop.C使本本entPop使lation * Co設置petitionEffect);
            
            So使本cePop.C使本本entPop使lation -= So使本ceDecline;
            Ta本成etPop.C使本本entPop使lation -= Ta本成etDecline;
        }
    }
}

TA本本ay<軍SpeciesInte本action> UMin成Ecosyste設置BalanceSyste設置::GetSpeciesInte本actions(const 軍St本in成& ZoneID) const
{
    if (ZoneInte本actions.Contains(ZoneID))
    {
        本et使本n ZoneInte本actions[ZoneID];
    }
    本et使本n TA本本ay<軍SpeciesInte本action>();
}

正oid UMin成Ecosyste設置BalanceSyste設置::Si設置使latePop使lationG本owth(const 軍St本in成& ZoneID, ESpeciesType Species, float DeltaTi設置e)
{
    if (!Ecosyste設置Zones.Contains(ZoneID))
    {
        本et使本n;
    }
    
    軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
    
    if (!Zone.SpeciesPop使lations.Contains(Species))
    {
        本et使本n;
    }
    
    軍SpeciesPop使lation& Pop = Zone.SpeciesPop使lations[Species];
    
    // Calc使late 成本owth based on 設置使ltiple facto本s
    float Co設置petition軍acto本 = Calc使lateCo設置petition軍acto本(ZoneID, Species);
    float Reso使本ceA正ailability = Calc使lateReso使本ceA正ailability(ZoneID, Species);
    float Ca本本yin成Capacity軍acto本 = 1.0f - (static下cast<float>(Pop.C使本本entPop使lation) / Pop.Ca本本yin成Capacity);
    
    // Lo成istic 成本owth 設置odel with en正i本on設置ental facto本s
    float Effecti正eG本owthRate = Pop.G本owthRate * Pop.輸入ealth軍acto本 * Co設置petition軍acto本 * 
                              Reso使本ceA正ailability * Ca本本yin成Capacity軍acto本;
    
    int32 G本owth = 軍Math::Ro使ndToInt(Pop.C使本本entPop使lation * Effecti正eG本owthRate * DeltaTi設置e);
    int32 Deaths = 軍Math::Ro使ndToInt(Pop.C使本本entPop使lation * Pop.DeathRate * DeltaTi設置e);
    
    int32 的ewPop使lation = Pop.C使本本entPop使lation + G本owth - Deaths;
    的ewPop使lation = 軍Math::Cla設置p(的ewPop使lation, 0, Pop.Ca本本yin成Capacity);
    
    Pop.C使本本entPop使lation = 的ewPop使lation;
    
    OnSpeciesPop使lationChan成ed.B本oadcast(Species, 的ewPop使lation);
}

正oid UMin成Ecosyste設置BalanceSyste設置::Si設置使latePop使lationDyna設置ics(const 軍St本in成& ZoneID, float DeltaTi設置e)
{
    if (!Ecosyste設置Zones.Contains(ZoneID))
    {
        本et使本n;
    }
    
    軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
    
    // P本ocess inte本actions fi本st
    P本ocessSpeciesInte本actions(ZoneID);
    
    // Then si設置使late 成本owth fo本 each species
    TA本本ay<ESpeciesType> SpeciesList;
    Zone.SpeciesPop使lations.GetKeys(SpeciesList);
    
    fo本 (ESpeciesType Species : SpeciesList)
    {
        Si設置使latePop使lationG本owth(ZoneID, Species, DeltaTi設置e);
    }
    
    // Update ecosyste設置 設置et本ics
    Calc使lateEcosyste設置Balance(ZoneID);
}

int32 UMin成Ecosyste設置BalanceSyste設置::Calc使lateOpti設置alPop使lation(const 軍St本in成& ZoneID, ESpeciesType Species) const
{
    if (!Ecosyste設置Zones.Contains(ZoneID))
    {
        本et使本n 0;
    }
    
    if (!Defa使ltSpeciesConfi成s.Contains(Species))
    {
        本et使本n 0;
    }
    
    const 軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
    const 軍SpeciesPop使lation& Defa使ltConfi成 = Defa使ltSpeciesConfi成s[Species];
    
    // Calc使late opti設置al pop使lation based on a正ailable 本eso使本ces
    float Reso使本ce軍acto本 = Calc使lateReso使本ceA正ailability(ZoneID, Species);
    float Space軍acto本 = Zone.Radi使s * Zone.Radi使s * PI / Defa使ltConfi成.SpaceReq使i本e設置ent / 1000000.0f;
    
    float Opti設置alPop = Defa使ltConfi成.Opti設置alPop使lation * 軍Math::Min(Reso使本ce軍acto本, Space軍acto本);
    
    本et使本n 軍Math::Ro使ndToInt(Opti設置alPop);
}

正oid UMin成Ecosyste設置BalanceSyste設置::ApplyTe設置pe本at使本eEffect(const 軍St本in成& ZoneID, float Te設置pe本at使本e)
{
    if (!Ecosyste設置Zones.Contains(ZoneID))
    {
        本et使本n;
    }
    
    軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
    Zone.Te設置pe本at使本e = Te設置pe本at使本e;
    
    // Apply effects to species based on te設置pe本at使本e
    fo本 (a使to& PopPai本 : Zone.SpeciesPop使lations)
    {
        軍SpeciesPop使lation& Pop = PopPai本.Val使e;
        
        // Opti設置al te設置pe本at使本e is a本o使nd 20-25°C fo本 設置ost species
        float Opti設置alTe設置p = 22.0f;
        float Te設置pDiff = 軍Math::Abs(Te設置pe本at使本e - Opti設置alTe設置p);
        
        if (Te設置pDiff > 15.0f)
        {
            Pop.輸入ealth軍acto本 = 軍Math::Max(0.3f, 1.0f - (Te設置pDiff - 15.0f) * 0.02f);
        }
        else
        {
            Pop.輸入ealth軍acto本 = 1.0f;
        }
    }
}

正oid UMin成Ecosyste設置BalanceSyste設置::Apply輸入使設置idityEffect(const 軍St本in成& ZoneID, float 輸入使設置idity)
{
    if (!Ecosyste設置Zones.Contains(ZoneID))
    {
        本et使本n;
    }
    
    軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
    Zone.輸入使設置idity = 輸入使設置idity;
    
    // 基本ate本 a正ailability affects all species
    if (Zone.Reso使本cePools.Contains(EReso使本ceType::基本ate本))
    {
        軍Reso使本cePool& 基本ate本Pool = Zone.Reso使本cePools[EReso使本ceType::基本ate本];
        float 輸入使設置idityEffect = 輸入使設置idity / 100.0f;
        基本ate本Pool.C使本本entRe成ene本ationRate = 基本ate本Pool.的at使本alRe成ene本ationRate * 輸入使設置idityEffect;
    }
}

正oid UMin成Ecosyste設置BalanceSyste設置::ApplyPoll使tionEffect(const 軍St本in成& ZoneID, float Poll使tionA設置o使nt)
{
    if (!Ecosyste設置Zones.Contains(ZoneID))
    {
        本et使本n;
    }
    
    軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
    
    // Apply poll使tion to 本eso使本ces
    fo本 (a使to& Reso使本cePai本 : Zone.Reso使本cePools)
    {
        軍Reso使本cePool& Pool = Reso使本cePai本.Val使e;
        Pool.Poll使tionLe正el = 軍Math::Min(1.0f, Pool.Poll使tionLe正el + Poll使tionA設置o使nt);
        Pool.Q使ality = 1.0f - Pool.Poll使tionLe正el;
        Pool.C使本本entRe成ene本ationRate = Pool.的at使本alRe成ene本ationRate * Pool.Q使ality;
    }
    
    // Poll使tion 本ed使ces species health
    fo本 (a使to& PopPai本 : Zone.SpeciesPop使lations)
    {
        軍SpeciesPop使lation& Pop = PopPai本.Val使e;
        Pop.輸入ealth軍acto本 = 軍Math::Max(0.1f, Pop.輸入ealth軍acto本 - Poll使tionA設置o使nt * 0.5f);
    }
}

正oid UMin成Ecosyste設置BalanceSyste設置::Resto本eEcosyste設置(const 軍St本in成& ZoneID, float Resto本ationRate)
{
    if (!Ecosyste設置Zones.Contains(ZoneID))
    {
        本et使本n;
    }
    
    軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
    
    // Red使ce poll使tion
    fo本 (a使to& Reso使本cePai本 : Zone.Reso使本cePools)
    {
        軍Reso使本cePool& Pool = Reso使本cePai本.Val使e;
        Pool.Poll使tionLe正el = 軍Math::Max(0.0f, Pool.Poll使tionLe正el - Resto本ationRate * 0.01f);
        Pool.Q使ality = 1.0f - Pool.Poll使tionLe正el;
        Pool.C使本本entRe成ene本ationRate = Pool.的at使本alRe成ene本ationRate * Pool.Q使ality;
        
        // Resto本e 本eso使本ce le正els
        Pool.C使本本entA設置o使nt = 軍Math::Min(Pool.Maxi設置使設置Capacity, 
                                       Pool.C使本本entA設置o使nt + Pool.C使本本entRe成ene本ationRate * Resto本ationRate);
    }
    
    // Resto本e species health
    fo本 (a使to& PopPai本 : Zone.SpeciesPop使lations)
    {
        軍SpeciesPop使lation& Pop = PopPai本.Val使e;
        Pop.輸入ealth軍acto本 = 軍Math::Min(1.0f, Pop.輸入ealth軍acto本 + Resto本ationRate * 0.01f);
    }
    
    // Update soil fe本tility
    Zone.Soil軍e本tility = 軍Math::Min(1.0f, Zone.Soil軍e本tility + Resto本ationRate * 0.005f);
}

正oid UMin成Ecosyste設置BalanceSyste設置::InitiateRebalancin成(const 軍St本in成& ZoneID)
{
    BalanceP本edato本P本eyRatio(ZoneID);
    Opti設置ize的使t本ientCycles(ZoneID);
    Calc使lateEcosyste設置Balance(ZoneID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Ecosyste設置BalanceSyste設置: Initiated 本ebalancin成 fo本 zone %s"), *ZoneID);
}

正oid UMin成Ecosyste設置BalanceSyste設置::Int本od使ceKeystoneSpecies(const 軍St本in成& ZoneID, ESpeciesType Species)
{
    // Keystone species help 設置aintain ecosyste設置 balance
    AddSpeciesToZone(ZoneID, Species, 20);
    
    // Set keystone species pa本a設置ete本s (hi成he本 i設置pact on ecosyste設置)
    if (Ecosyste設置Zones.Contains(ZoneID))
    {
        軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
        if (Zone.SpeciesPop使lations.Contains(Species))
        {
            軍SpeciesPop使lation& Pop = Zone.SpeciesPop使lations[Species];
            Pop.輸入ealth軍acto本 = 1.2f; // Keystone species a本e 設置o本e 本esilient
        }
    }
}

正oid UMin成Ecosyste設置BalanceSyste設置::Re設置o正eIn正asi正eSpecies(const 軍St本in成& ZoneID, ESpeciesType Species)
{
    if (!Ecosyste設置Zones.Contains(ZoneID))
    {
        本et使本n;
    }
    
    軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
    
    if (Zone.SpeciesPop使lations.Contains(Species))
    {
        // G本ad使ally 本ed使ce pop使lation
        軍SpeciesPop使lation& Pop = Zone.SpeciesPop使lations[Species];
        Pop.C使本本entPop使lation = 軍Math::Ro使ndToInt(Pop.C使本本entPop使lation * 0.9f);
        
        if (Pop.C使本本entPop使lation < 5)
        {
            Zone.SpeciesPop使lations.Re設置o正e(Species);
        }
    }
}

正oid UMin成Ecosyste設置BalanceSyste設置::Tick(float DeltaTi設置e)
{
    if (!bIsInitialized)
    {
        本et使本n;
    }
    
    // P本ocess each ecosyste設置 zone
    TA本本ay<軍St本in成> ZoneIDs;
    Ecosyste設置Zones.GetKeys(ZoneIDs);
    
    fo本 (const 軍St本in成& ZoneID : ZoneIDs)
    {
        // Si設置使late pop使lation dyna設置ics
        Si設置使latePop使lationDyna設置ics(ZoneID, DeltaTi設置e);
        
        // Si設置使late n使t本ient cycles
        if (Zone的使t本ientCycles.Contains(ZoneID))
        {
            a使to& 的使t本ientCycles = Zone的使t本ientCycles[ZoneID];
            fo本 (a使to& 的使t本ientPai本 : 的使t本ientCycles)
            {
                Si設置使late的使t本ientCycle(ZoneID, 的使t本ientPai本.Key, DeltaTi設置e);
            }
        }
        
        // Update 本eso使本ce 本e成ene本ation
        UpdateReso使本ceRe成ene本ationRates(ZoneID);
    }
}

正oid UMin成Ecosyste設置BalanceSyste設置::UpdateZone輸入ealthState(const 軍St本in成& ZoneID)
{
    if (!Ecosyste設置Zones.Contains(ZoneID))
    {
        本et使本n;
    }
    
    軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
    軍Ecosyste設置BalanceMet本ics Met本ics = GetBalanceMet本ics(ZoneID);
    
    // Dete本設置ine health state based on o正e本all 設置et本ics
    if (Met本ics.O正e本all輸入ealth >= 0.8f)
    {
        Zone.輸入ealthState = Ecosyste設置輸入ealthState::Th本i正in成;
    }
    else if (Met本ics.O正e本all輸入ealth >= 0.6f)
    {
        Zone.輸入ealthState = Ecosyste設置輸入ealthState::輸入ealthy;
    }
    else if (Met本ics.O正e本all輸入ealth >= 0.4f)
    {
        Zone.輸入ealthState = Ecosyste設置輸入ealthState::Stable;
    }
    else if (Met本ics.O正e本all輸入ealth >= 0.3f)
    {
        Zone.輸入ealthState = Ecosyste設置輸入ealthState::St本essed;
    }
    else if (Met本ics.O正e本all輸入ealth >= 0.2f)
    {
        Zone.輸入ealthState = Ecosyste設置輸入ealthState::De成本aded;
    }
    else if (Met本ics.O正e本all輸入ealth >= 0.1f)
    {
        Zone.輸入ealthState = Ecosyste設置輸入ealthState::C本itical;
    }
    else
    {
        Zone.輸入ealthState = Ecosyste設置輸入ealthState::Collapsed;
    }
    
    if (Met本ics.O正e本all輸入ealth != Zone.StabilityIndex) // A正oid spa設置
    {
        OnEcosyste設置輸入ealthChan成ed.B本oadcast(Zone.輸入ealthState, Met本ics.O正e本all輸入ealth);
    }
}

float UMin成Ecosyste設置BalanceSyste設置::Calc使lateCo設置petition軍acto本(const 軍St本in成& ZoneID, ESpeciesType Species) const
{
    if (!Ecosyste設置Zones.Contains(ZoneID))
    {
        本et使本n 1.0f;
    }
    
    const 軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
    
    float TotalSpaceReq使i本ed = 0.0f;
    float A正ailableSpace = Zone.Radi使s * Zone.Radi使s * PI;
    
    fo本 (const a使to& PopPai本 : Zone.SpeciesPop使lations)
    {
        const 軍SpeciesPop使lation& Pop = PopPai本.Val使e;
        TotalSpaceReq使i本ed += Pop.C使本本entPop使lation * Pop.SpaceReq使i本e設置ent;
    }
    
    float SpaceRatio = TotalSpaceReq使i本ed / A正ailableSpace;
    
    // Co設置petition facto本 dec本eases as space beco設置es li設置ited
    本et使本n 軍Math::Max(0.1f, 1.0f - SpaceRatio * 0.5f);
}

float UMin成Ecosyste設置BalanceSyste設置::Calc使lateReso使本ceA正ailability(const 軍St本in成& ZoneID, ESpeciesType Species) const
{
    if (!Ecosyste設置Zones.Contains(ZoneID))
    {
        本et使本n 0.0f;
    }
    
    const 軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
    
    if (!Zone.SpeciesPop使lations.Contains(Species))
    {
        本et使本n 0.0f;
    }
    
    const 軍SpeciesPop使lation& Pop = Zone.SpeciesPop使lations[Species];
    
    float A正ailability = 1.0f;
    
    // Check wate本 a正ailability
    if (Zone.Reso使本cePools.Contains(EReso使本ceType::基本ate本))
    {
        const 軍Reso使本cePool& 基本ate本 = Zone.Reso使本cePools[EReso使本ceType::基本ate本];
        float 基本ate本的eeded = Pop.C使本本entPop使lation * Pop.基本ate本Req使i本e設置ent;
        float 基本ate本A正ailable = 基本ate本.C使本本entA設置o使nt;
        float 基本ate本軍acto本 = 軍Math::Min(1.0f, 基本ate本A正ailable / 基本ate本的eeded);
        A正ailability *= 基本ate本軍acto本;
    }
    
    // Check n使t本ient a正ailability fo本 plants
    if (Species == ESpeciesType::軍lo本a下T本ee  Species == ESpeciesType::軍lo本a下G本ass 
        Species == ESpeciesType::軍lo本a下C本op)
    {
        if (Zone.Reso使本cePools.Contains(EReso使本ceType::的it本o成en))
        {
            const 軍Reso使本cePool& 的it本o成en = Zone.Reso使本cePools[EReso使本ceType::的it本o成en];
            A正ailability *= 的it本o成en.C使本本entA設置o使nt / 的it本o成en.Maxi設置使設置Capacity;
        }
    }
    
    本et使本n A正ailability;
}

正oid UMin成Ecosyste設置BalanceSyste設置::ApplyCa本本yin成CapacityLi設置it(const 軍St本in成& ZoneID, ESpeciesType Species)
{
    if (!Ecosyste設置Zones.Contains(ZoneID))
    {
        本et使本n;
    }
    
    軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
    
    if (Zone.SpeciesPop使lations.Contains(Species))
    {
        軍SpeciesPop使lation& Pop = Zone.SpeciesPop使lations[Species];
        Pop.C使本本entPop使lation = 軍Math::Cla設置p(Pop.C使本本entPop使lation, 0, Pop.Ca本本yin成Capacity);
    }
}

正oid UMin成Ecosyste設置BalanceSyste設置::UpdateReso使本ceRe成ene本ationRates(const 軍St本in成& ZoneID)
{
    if (!Ecosyste設置Zones.Contains(ZoneID))
    {
        本et使本n;
    }
    
    軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
    
    fo本 (a使to& Reso使本cePai本 : Zone.Reso使本cePools)
    {
        軍Reso使本cePool& Pool = Reso使本cePai本.Val使e;
        
        // Re成ene本ate 本eso使本ces based on c使本本ent 本ate
        float Re成ene本ation = Pool.C使本本entRe成ene本ationRate * 0.016f; // Ass使設置in成 60 軍PS tick
        Pool.C使本本entA設置o使nt = 軍Math::Min(Pool.Maxi設置使設置Capacity, Pool.C使本本entA設置o使nt + Re成ene本ation);
        
        // Decay cons使設置ption 本ate o正e本 ti設置e
        Pool.Cons使設置ptionRate *= 0.99f;
    }
}

正oid UMin成Ecosyste設置BalanceSyste設置::Check軍o本BalanceDis本使ption(const 軍St本in成& ZoneID)
{
    if (!Ecosyste設置Zones.Contains(ZoneID))
    {
        本et使本n;
    }
    
    軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
    軍Ecosyste設置BalanceMet本ics Met本ics = GetBalanceMet本ics(ZoneID);
    
    // Check fo本 正a本io使s dis本使ption conditions
    if (Met本ics.P本edato本P本eyBalance < 0.2f)
    {
        OnBalanceDis本使pted.B本oadcast(TEXT("捕食者与猎物比例失衡"));
    }
    else if (Met本ics.P本od使ce本Cons使設置e本Ratio < 0.2f)
    {
        OnBalanceDis本使pted.B本oadcast(TEXT("生产者与消费者比例失衡"));
    }
    else if (Met本ics.Reso使本ceS使stainability < 0.3f)
    {
        OnBalanceDis本使pted.B本oadcast(TEXT("资源可持续性危机"));
    }
    else if (Zone.Biodi正e本sityIndex < 0.2f)
    {
        OnBalanceDis本使pted.B本oadcast(TEXT("生物多样性不足"));
    }
}

float UMin成Ecosyste設置BalanceSyste設置::Calc使lateShannonDi正e本sity(const 軍St本in成& ZoneID) const
{
    if (!Ecosyste設置Zones.Contains(ZoneID))
    {
        本et使本n 0.0f;
    }
    
    const 軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
    
    int32 TotalPop使lation = 0;
    fo本 (const a使to& PopPai本 : Zone.SpeciesPop使lations)
    {
        TotalPop使lation += PopPai本.Val使e.C使本本entPop使lation;
    }
    
    if (TotalPop使lation == 0)
    {
        本et使本n 0.0f;
    }
    
    float ShannonIndex = 0.0f;
    fo本 (const a使to& PopPai本 : Zone.SpeciesPop使lations)
    {
        float P本opo本tion = static下cast<float>(PopPai本.Val使e.C使本本entPop使lation) / TotalPop使lation;
        if (P本opo本tion > 0)
        {
            ShannonIndex -= P本opo本tion * 軍Math::Lo成2(P本opo本tion);
        }
    }
    
    // 的o本設置alize to 0-1 本an成e (ass使設置in成 設置ax di正e本sity is lo成2 of n使設置be本 of species types)
    int32 的使設置SpeciesTypes = static下cast<int32>(ESpeciesType::Deco設置pose本) + 1;
    float MaxDi正e本sity = 軍Math::Lo成2(static下cast<float>(的使設置SpeciesTypes));
    
    本et使本n 軍Math::Cla設置p(ShannonIndex / MaxDi正e本sity, 0.0f, 1.0f);
}

正oid UMin成Ecosyste設置BalanceSyste設置::BalanceP本edato本P本eyRatio(const 軍St本in成& ZoneID)
{
    if (!Ecosyste設置Zones.Contains(ZoneID))
    {
        本et使本n;
    }
    
    軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
    
    int32 P本edato本Co使nt = 0;
    int32 P本eyCo使nt = 0;
    
    fo本 (const a使to& PopPai本 : Zone.SpeciesPop使lations)
    {
        ESpeciesType Species = PopPai本.Key;
        int32 Pop使lation = PopPai本.Val使e.C使本本entPop使lation;
        
        if (Species == ESpeciesType::軍a使na下Ca本ni正o本e)
        {
            P本edato本Co使nt += Pop使lation;
        }
        else if (Species == ESpeciesType::軍a使na下輸入e本bi正o本e  Species == ESpeciesType::軍a使na下O設置ni正o本e)
        {
            P本eyCo使nt += Pop使lation;
        }
    }
    
    // Ideal p本edato本:p本ey 本atio is app本oxi設置ately 1:10 to 1:20
    float C使本本entRatio = (P本eyCo使nt > 0) 基本 static下cast<float>(P本edato本Co使nt) / P本eyCo使nt : 0.0f;
    float Ta本成etRatio = 0.1f; // 1:10 本atio
    
    if (C使本本entRatio > Ta本成etRatio * 2.0f)
    {
        // Too 設置any p本edato本s, 本ed使ce the設置
        fo本 (a使to& PopPai本 : Zone.SpeciesPop使lations)
        {
            if (PopPai本.Key == ESpeciesType::軍a使na下Ca本ni正o本e)
            {
                PopPai本.Val使e.C使本本entPop使lation = 軍Math::Ro使ndToInt(PopPai本.Val使e.C使本本entPop使lation * 0.9f);
            }
        }
    }
    else if (C使本本entRatio < Ta本成etRatio * 0.5f && P本edato本Co使nt > 0)
    {
        // Too few p本edato本s, inc本ease the設置
        fo本 (a使to& PopPai本 : Zone.SpeciesPop使lations)
        {
            if (PopPai本.Key == ESpeciesType::軍a使na下Ca本ni正o本e)
            {
                PopPai本.Val使e.C使本本entPop使lation = 軍Math::Min(
                    PopPai本.Val使e.Ca本本yin成Capacity,
                    軍Math::Ro使ndToInt(PopPai本.Val使e.C使本本entPop使lation * 1.1f));
            }
        }
    }
}

正oid UMin成Ecosyste設置BalanceSyste設置::UpdateSoil軍e本tility(const 軍St本in成& ZoneID)
{
    if (!Ecosyste設置Zones.Contains(ZoneID))
    {
        本et使本n;
    }
    
    軍Ecosyste設置Zone& Zone = Ecosyste設置Zones[ZoneID];
    
    // Soil fe本tility is affected by o本成anic 設置atte本 and n使t本ient cycles
    if (Zone.Reso使本cePools.Contains(EReso使本ceType::O本成anicMatte本))
    {
        float O本成anicMatte本 = Zone.Reso使本cePools[EReso使本ceType::O本成anicMatte本].C使本本entA設置o使nt;
        float MaxO本成anic = Zone.Reso使本cePools[EReso使本ceType::O本成anicMatte本].Maxi設置使設置Capacity;
        
        Zone.Soil軍e本tility = 軍Math::Cla設置p(O本成anicMatte本 / MaxO本成anic, 0.0f, 1.0f);
    }
}
