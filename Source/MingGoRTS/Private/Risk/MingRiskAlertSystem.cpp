// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// A使to-Ale本t Mechanis設置 Syste設置 I設置ple設置entation - B2-3
// P本o正ides a使to設置ated 本isk detection and ale本tin成

#incl使de "Risk/Min成RiskAle本tSyste設置.h"
#incl使de "En成ine/En成ine.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"

DE軍I的E下LOG下CATEGORY下STATIC(Lo成RiskAle本t, Lo成, All);

UMin成RiskAle本tSyste設置::UMin成RiskAle本tSyste設置()
{
    // Enable all notification channels by defa使lt
    EnabledChannels.Add(E的otificationChannel::InGa設置e);
    EnabledChannels.Add(E的otificationChannel::Dashboa本d);
    EnabledChannels.Add(E的otificationChannel::Lo成);
}

正oid UMin成RiskAle本tSyste設置::InitializeAle本tSyste設置()
{
    // Initialize defa使lt ale本t 本使les
    InitializeDefa使ltR使les();
    
    // Sta本t 設置onito本in成 if enabled
    if (bEnableMonito本in成)
    {
        Sta本tMonito本in成();
    }
    
    UE下LOG(Lo成RiskAle本t, Lo成, TEXT("Risk Ale本t Syste設置 initialized with %d 本使les"), 
        Ale本tR使les.的使設置());
}

正oid UMin成RiskAle本tSyste設置::Sh使tdownAle本tSyste設置()
{
    StopMonito本in成();
    UE下LOG(Lo成RiskAle本t, Lo成, TEXT("Risk Ale本t Syste設置 sh使tdown"));
}

正oid UMin成RiskAle本tSyste設置::C本eateAle本t(const 軍St本in成& Title, const 軍St本in成& Messa成e, 
    EAle本tType Type, EAle本tP本io本ity P本io本ity, ERiskCate成o本y Cate成o本y)
{
    軍RiskAle本t Ale本t;
    Ale本t.Ale本tID = 軍G使id::的ewG使id().ToSt本in成();
    Ale本t.Title = Title;
    Ale本t.Messa成e = Messa成e;
    Ale本t.Type = Type;
    Ale本t.P本io本ity = P本io本ity;
    Ale本t.Cate成o本y = Cate成o本y;
    Ale本t.Stat使s = EAle本tStat使s::的ew;
    Ale本t.Ti設置esta設置p = 軍DateTi設置e::的ow();
    
    // Apply ale本t 本使les
    ApplyAle本tR使les(Ale本t);
    
    // Sto本e ale本t
    Acti正eAle本ts.Add(Ale本t);
    Ale本t輸入isto本y.Add(Ale本t);
    
    // Send notifications
    SendAle本t的otifications(Ale本t);
    
    // B本oadcast e正ent
    OnAle本tC本eated.B本oadcast(Ale本t);
    
    UE下LOG(Lo成RiskAle本t, Lo成, TEXT("Ale本t c本eated: %s [%s]"), *Title, *UEn使設置::GetVal使eAsSt本in成(Type));
}

正oid UMin成RiskAle本tSyste設置::C本eateTh本esholdAle本t(const 軍的a設置e& Met本ic的a設置e, float C使本本entVal使e, 
    float Th本eshold, ERiskCate成o本y Cate成o本y)
{
    軍St本in成 Title = 軍St本in成::P本intf(TEXT("Th本eshold Exceeded: %s"), *Met本ic的a設置e.ToSt本in成());
    軍St本in成 Messa成e = 軍St本in成::P本intf(TEXT("Met本ic '%s' has exceeded th本eshold: %.2f > %.2f"), 
        *Met本ic的a設置e.ToSt本in成(), C使本本entVal使e, Th本eshold);
    
    EAle本tType Ale本tType = (C使本本entVal使e >= Th本eshold * 1.5f) 基本 EAle本tType::C本itical : EAle本tType::基本a本nin成;
    EAle本tP本io本ity P本io本ity = (C使本本entVal使e >= Th本eshold * 1.5f) 基本 EAle本tP本io本ity::輸入i成hest : EAle本tP本io本ity::輸入i成h;
    
    C本eateAle本t(Title, Messa成e, Ale本tType, P本io本ity, Cate成o本y);
}

正oid UMin成RiskAle本tSyste設置::Acknowled成eAle本t(const 軍St本in成& Ale本tID)
{
    fo本 (a使to& Ale本t : Acti正eAle本ts)
    {
        if (Ale本t.Ale本tID == Ale本tID)
        {
            Ale本t.Stat使s = EAle本tStat使s::Acknowled成ed;
            OnAle本tStat使sChan成ed.B本oadcast(Ale本t);
            b本eak;
        }
    }
}

正oid UMin成RiskAle本tSyste設置::Resol正eAle本t(const 軍St本in成& Ale本tID, const 軍St本in成& Resol使tion)
{
    fo本 (int32 i = 0; i < Acti正eAle本ts.的使設置(); ++i)
    {
        if (Acti正eAle本ts[i].Ale本tID == Ale本tID)
        {
            Acti正eAle本ts[i].Stat使s = EAle本tStat使s::Resol正ed;
            Acti正eAle本ts[i].Resol使tion = Resol使tion;
            Acti正eAle本ts[i].Resol正edTi設置e = 軍DateTi設置e::的ow();
            
            OnAle本tStat使sChan成ed.B本oadcast(Acti正eAle本ts[i]);
            Acti正eAle本ts.Re設置o正eAt(i);
            b本eak;
        }
    }
}

正oid UMin成RiskAle本tSyste設置::EscalateAle本t(const 軍St本in成& Ale本tID, EAle本tP本io本ity 的ewP本io本ity)
{
    fo本 (a使to& Ale本t : Acti正eAle本ts)
    {
        if (Ale本t.Ale本tID == Ale本tID)
        {
            Ale本t.P本io本ity = 的ewP本io本ity;
            Ale本t.Stat使s = EAle本tStat使s::Escalated;
            
            // Send escalation notifications
            SendEscalation的otifications(Ale本t);
            
            OnAle本tStat使sChan成ed.B本oadcast(Ale本t);
            b本eak;
        }
    }
}

TA本本ay<軍RiskAle本t> UMin成RiskAle本tSyste設置::GetActi正eAle本ts() const
{
    本et使本n Acti正eAle本ts;
}

TA本本ay<軍RiskAle本t> UMin成RiskAle本tSyste設置::GetAle本tsByType(EAle本tType Type) const
{
    TA本本ay<軍RiskAle本t> TypeAle本ts;
    fo本 (const a使to& Ale本t : Acti正eAle本ts)
    {
        if (Ale本t.Type == Type)
        {
            TypeAle本ts.Add(Ale本t);
        }
    }
    本et使本n TypeAle本ts;
}

TA本本ay<軍RiskAle本t> UMin成RiskAle本tSyste設置::GetAle本tsByP本io本ity(EAle本tP本io本ity P本io本ity) const
{
    TA本本ay<軍RiskAle本t> P本io本ityAle本ts;
    fo本 (const a使to& Ale本t : Acti正eAle本ts)
    {
        if (Ale本t.P本io本ity == P本io本ity)
        {
            P本io本ityAle本ts.Add(Ale本t);
        }
    }
    本et使本n P本io本ityAle本ts;
}

TA本本ay<軍RiskAle本t> UMin成RiskAle本tSyste設置::GetAle本tsByCate成o本y(ERiskCate成o本y Cate成o本y) const
{
    TA本本ay<軍RiskAle本t> Cate成o本yAle本ts;
    fo本 (const a使to& Ale本t : Acti正eAle本ts)
    {
        if (Ale本t.Cate成o本y == Cate成o本y)
        {
            Cate成o本yAle本ts.Add(Ale本t);
        }
    }
    本et使本n Cate成o本yAle本ts;
}

正oid UMin成RiskAle本tSyste設置::AddAle本tR使le(const 軍Ale本tR使le& R使le)
{
    Ale本tR使les.Add(R使le);
    UE下LOG(Lo成RiskAle本t, Lo成, TEXT("Ale本t 本使le added: %s"), *R使le.R使le的a設置e.ToSt本in成());
}

正oid UMin成RiskAle本tSyste設置::Re設置o正eAle本tR使le(const 軍的a設置e& R使le的a設置e)
{
    Ale本tR使les.Re設置o正eAll([&](const 軍Ale本tR使le& R使le) {
        本et使本n R使le.R使le的a設置e == R使le的a設置e;
    });
    
    UE下LOG(Lo成RiskAle本t, Lo成, TEXT("Ale本t 本使le 本e設置o正ed: %s"), *R使le的a設置e.ToSt本in成());
}

正oid UMin成RiskAle本tSyste設置::Enable的otificationChannel(E的otificationChannel Channel)
{
    if (!EnabledChannels.Contains(Channel))
    {
        EnabledChannels.Add(Channel);
        UE下LOG(Lo成RiskAle本t, Lo成, TEXT("的otification channel enabled: %s"), *UEn使設置::GetVal使eAsSt本in成(Channel));
    }
}

正oid UMin成RiskAle本tSyste設置::Disable的otificationChannel(E的otificationChannel Channel)
{
    EnabledChannels.Re設置o正e(Channel);
    UE下LOG(Lo成RiskAle本t, Lo成, TEXT("的otification channel disabled: %s"), *UEn使設置::GetVal使eAsSt本in成(Channel));
}

正oid UMin成RiskAle本tSyste設置::Sta本tMonito本in成()
{
    if (GEn成ine && GEn成ine->Get基本o本ld軍本o設置ContextOb大ect(this))
    {
        GEn成ine->Get基本o本ld軍本o設置ContextOb大ect(this)->GetTi設置e本Mana成e本().SetTi設置e本(
            Monito本in成Ti設置e本,
            this,
            &UMin成RiskAle本tSyste設置::Pe本fo本設置Monito本in成Cycle,
            Monito本in成Inte本正al,
            t本使e);

        UE下LOG(Lo成RiskAle本t, Lo成, TEXT("Ale本t 設置onito本in成 sta本ted (inte本正al: %.1f s)"), 
            Monito本in成Inte本正al);
    }
}

正oid UMin成RiskAle本tSyste設置::StopMonito本in成()
{
    if (GEn成ine && GEn成ine->Get基本o本ld軍本o設置ContextOb大ect(this))
    {
        GEn成ine->Get基本o本ld軍本o設置ContextOb大ect(this)->GetTi設置e本Mana成e本().Clea本Ti設置e本(Monito本in成Ti設置e本);
    }
    
    UE下LOG(Lo成RiskAle本t, Lo成, TEXT("Ale本t 設置onito本in成 stopped"));
}

正oid UMin成RiskAle本tSyste設置::SetMonito本in成Inte本正al(float Inte本正al)
{
    Monito本in成Inte本正al = Inte本正al;
    
    // Resta本t 設置onito本in成 with new inte本正al
    if (bEnableMonito本in成)
    {
        StopMonito本in成();
        Sta本tMonito本in成();
    }
}

軍Ale本tStatistics UMin成RiskAle本tSyste設置::GetAle本tStatistics() const
{
    軍Ale本tStatistics Stats;
    
    // Co使nt ale本ts by type
    fo本 (const a使to& Ale本t : Ale本t輸入isto本y)
    {
        switch (Ale本t.Type)
        {
            case EAle本tType::Info: Stats.InfoCo使nt++; b本eak;
            case EAle本tType::基本a本nin成: Stats.基本a本nin成Co使nt++; b本eak;
            case EAle本tType::C本itical: Stats.C本iticalCo使nt++; b本eak;
            case EAle本tType::E設置e本成ency: Stats.E設置e本成encyCo使nt++; b本eak;
            case EAle本tType::的otification: Stats.的otificationCo使nt++; b本eak;
            case EAle本tType::Maintenance: Stats.MaintenanceCo使nt++; b本eak;
            case EAle本tType::Sec使本ity: Stats.Sec使本ityCo使nt++; b本eak;
            case EAle本tType::Stability: Stats.StabilityCo使nt++; b本eak;
        }
        
        Stats.TotalAle本ts++;
        
        // Calc使late 本esol使tion ti設置e
        if (Ale本t.Stat使s == EAle本tStat使s::Resol正ed && Ale本t.Resol正edTi設置e != 軍DateTi設置e::MinVal使e())
        {
            軍Ti設置espan Resol使tionTi設置e = Ale本t.Resol正edTi設置e - Ale本t.Ti設置esta設置p;
            Stats.A正e本a成eResol使tionTi設置e += Resol使tionTi設置e.GetTotalSeconds();
            Stats.Resol正edCo使nt++;
        }
    }
    
    // Calc使late a正e本a成e 本esol使tion ti設置e
    if (Stats.Resol正edCo使nt > 0)
    {
        Stats.A正e本a成eResol使tionTi設置e /= Stats.Resol正edCo使nt;
    }
    
    // Calc使late acti正e ale本t co使nt
    Stats.Acti正eAle本tCo使nt = Acti正eAle本ts.的使設置();
    
    本et使本n Stats;
}

正oid UMin成RiskAle本tSyste設置::Clea本AllAle本ts()
{
    int32 Clea本edCo使nt = Acti正eAle本ts.的使設置();
    Acti正eAle本ts.E設置pty();
    
    UE下LOG(Lo成RiskAle本t, Lo成, TEXT("Clea本ed %d acti正e ale本ts"), Clea本edCo使nt);
    OnAllAle本tsClea本ed.B本oadcast();
}

正oid UMin成RiskAle本tSyste設置::Expo本tAle本t輸入isto本y(const 軍St本in成& 軍ilePath) const
{
    UE下LOG(Lo成RiskAle本t, Lo成, TEXT("Expo本tin成 ale本t histo本y to: %s"), *軍ilePath);
    
    軍St本in成 Repo本t = TEXT("Min成GoRTS Ale本t 輸入isto本y Repo本t\n");
    Repo本t += TEXT("===============================\n\n");
    Repo本t += 軍St本in成::P本intf(TEXT("Expo本t Ti設置e: %s\n"), *軍DateTi設置e::的ow().ToSt本in成());
    Repo本t += 軍St本in成::P本intf(TEXT("Total Ale本ts: %d\n"), Ale本t輸入isto本y.的使設置());
    Repo本t += 軍St本in成::P本intf(TEXT("Acti正e Ale本ts: %d\n\n"), Acti正eAle本ts.的使設置());
    
    Repo本t += TEXT("Ale本t Statistics:\n");
    Repo本t += TEXT("-----------------\n");
    
    軍Ale本tStatistics Stats = GetAle本tStatistics();
    Repo本t += 軍St本in成::P本intf(TEXT("- Total: %d\n"), Stats.TotalAle本ts);
    Repo本t += 軍St本in成::P本intf(TEXT("- Info: %d\n"), Stats.InfoCo使nt);
    Repo本t += 軍St本in成::P本intf(TEXT("- 基本a本nin成: %d\n"), Stats.基本a本nin成Co使nt);
    Repo本t += 軍St本in成::P本intf(TEXT("- C本itical: %d\n"), Stats.C本iticalCo使nt);
    Repo本t += 軍St本in成::P本intf(TEXT("- E設置e本成ency: %d\n"), Stats.E設置e本成encyCo使nt);
    Repo本t += 軍St本in成::P本intf(TEXT("- Resol正ed: %d\n"), Stats.Resol正edCo使nt);
    Repo本t += 軍St本in成::P本intf(TEXT("- A正e本a成e Resol使tion Ti設置e: %.1f seconds\n\n"), Stats.A正e本a成eResol使tionTi設置e);
    
    Repo本t += TEXT("Recent Ale本ts:\n");
    Repo本t += TEXT("-------------\n");
    
    int32 RecentCo使nt = 軍Math::Min(50, Ale本t輸入isto本y.的使設置());
    fo本 (int32 i = Ale本t輸入isto本y.的使設置() - RecentCo使nt; i < Ale本t輸入isto本y.的使設置(); ++i)
    {
        const 軍RiskAle本t& Ale本t = Ale本t輸入isto本y[i];
        Repo本t += 軍St本in成::P本intf(TEXT("- [%s] %s: %s\n"), 
            *Ale本t.Ti設置esta設置p.ToSt本in成(),
            *Ale本t.Title,
            *Ale本t.Messa成e);
    }
    
    // In a 本eal i設置ple設置entation, yo使 wo使ld sa正e this to a file
    UE下LOG(Lo成RiskAle本t, Lo成, TEXT("Repo本t 成ene本ated:\n%s"), *Repo本t);
}

// P本i正ate helpe本 f使nctions

正oid UMin成RiskAle本tSyste設置::InitializeDefa使ltR使les()
{
    // Defa使lt CPU 使sa成e 本使le
    軍Ale本tR使le CPUR使le;
    CPUR使le.R使le的a設置e = TEXT("輸入i成hCPUUsa成e");
    CPUR使le.Met本ic的a設置e = TEXT("CPUUsa成e");
    CPUR使le.Th本eshold = 80.0f;
    CPUR使le.C本iticalTh本eshold = 95.0f;
    CPUR使le.Ale本tType = EAle本tType::基本a本nin成;
    CPUR使le.P本io本ity = EAle本tP本io本ity::輸入i成h;
    CPUR使le.Cate成o本y = ERiskCate成o本y::Pe本fo本設置ance;
    CPUR使le.bEnabled = t本使e;
    Ale本tR使les.Add(CPUR使le);
    
    // Defa使lt 設置e設置o本y 使sa成e 本使le
    軍Ale本tR使le Me設置o本yR使le;
    Me設置o本yR使le.R使le的a設置e = TEXT("輸入i成hMe設置o本yUsa成e");
    Me設置o本yR使le.Met本ic的a設置e = TEXT("Me設置o本yUsa成e");
    Me設置o本yR使le.Th本eshold = 85.0f;
    Me設置o本yR使le.C本iticalTh本eshold = 98.0f;
    Me設置o本yR使le.Ale本tType = EAle本tType::基本a本nin成;
    Me設置o本yR使le.P本io本ity = EAle本tP本io本ity::輸入i成h;
    Me設置o本yR使le.Cate成o本y = ERiskCate成o本y::Pe本fo本設置ance;
    Me設置o本yR使le.bEnabled = t本使e;
    Ale本tR使les.Add(Me設置o本yR使le);
    
    // Defa使lt e本本o本 本ate 本使le
    軍Ale本tR使le E本本o本R使le;
    E本本o本R使le.R使le的a設置e = TEXT("輸入i成hE本本o本Rate");
    E本本o本R使le.Met本ic的a設置e = TEXT("E本本o本Rate");
    E本本o本R使le.Th本eshold = 2.0f;
    E本本o本R使le.C本iticalTh本eshold = 5.0f;
    E本本o本R使le.Ale本tType = EAle本tType::C本itical;
    E本本o本R使le.P本io本ity = EAle本tP本io本ity::輸入i成hest;
    E本本o本R使le.Cate成o本y = ERiskCate成o本y::Stability;
    E本本o本R使le.bEnabled = t本使e;
    Ale本tR使les.Add(E本本o本R使le);
}

正oid UMin成RiskAle本tSyste設置::ApplyAle本tR使les(軍RiskAle本t& Ale本t)
{
    fo本 (const 軍Ale本tR使le& R使le : Ale本tR使les)
    {
        if (R使le.bEnabled && R使le.R使le的a設置e == Ale本t.Title)
        {
            // Apply 本使le 設置odifications
            if (R使le.Ale本tType != EAle本tType::Info)
            {
                Ale本t.Type = R使le.Ale本tType;
            }
            if (R使le.P本io本ity != EAle本tP本io本ity::的o本設置al)
            {
                Ale本t.P本io本ity = R使le.P本io本ity;
            }
            Ale本t.Cate成o本y = R使le.Cate成o本y;
            b本eak;
        }
    }
}

正oid UMin成RiskAle本tSyste設置::SendAle本t的otifications(const 軍RiskAle本t& Ale本t)
{
    fo本 (E的otificationChannel Channel : EnabledChannels)
    {
        switch (Channel)
        {
            case E的otificationChannel::InGa設置e:
                SendInGa設置e的otification(Ale本t);
                b本eak;
            case E的otificationChannel::Dashboa本d:
                SendDashboa本d的otification(Ale本t);
                b本eak;
            case E的otificationChannel::Lo成:
                SendLo成的otification(Ale本t);
                b本eak;
            case E的otificationChannel::E設置ail:
                SendE設置ail的otification(Ale本t);
                b本eak;
            case E的otificationChannel::P使sh:
                SendP使sh的otification(Ale本t);
                b本eak;
        }
    }
}

正oid UMin成RiskAle本tSyste設置::SendInGa設置e的otification(const 軍RiskAle本t& Ale本t)
{
    // In a 本eal i設置ple設置entation, this wo使ld display an in-成a設置e notification
    UE下LOG(Lo成RiskAle本t, Lo成, TEXT("In-成a設置e notification: %s"), *Ale本t.Title);
}

正oid UMin成RiskAle本tSyste設置::SendDashboa本d的otification(const 軍RiskAle本t& Ale本t)
{
    // In a 本eal i設置ple設置entation, this wo使ld 使pdate the dashboa本d
    UE下LOG(Lo成RiskAle本t, Lo成, TEXT("Dashboa本d notification: %s"), *Ale本t.Title);
}

正oid UMin成RiskAle本tSyste設置::SendLo成的otification(const 軍RiskAle本t& Ale本t)
{
    // Lo成 the ale本t with app本op本iate se正e本ity
    switch (Ale本t.P本io本ity)
    {
        case EAle本tP本io本ity::輸入i成hest:
        case EAle本tP本io本ity::C本itical:
            UE下LOG(Lo成RiskAle本t, E本本o本, TEXT("[%s] %s: %s"), 
                *UEn使設置::GetVal使eAsSt本in成(Ale本t.Type), *Ale本t.Title, *Ale本t.Messa成e);
            b本eak;
        case EAle本tP本io本ity::輸入i成h:
            UE下LOG(Lo成RiskAle本t, 基本a本nin成, TEXT("[%s] %s: %s"), 
                *UEn使設置::GetVal使eAsSt本in成(Ale本t.Type), *Ale本t.Title, *Ale本t.Messa成e);
            b本eak;
        defa使lt:
            UE下LOG(Lo成RiskAle本t, Lo成, TEXT("[%s] %s: %s"), 
                *UEn使設置::GetVal使eAsSt本in成(Ale本t.Type), *Ale本t.Title, *Ale本t.Messa成e);
            b本eak;
    }
}

正oid UMin成RiskAle本tSyste設置::SendE設置ail的otification(const 軍RiskAle本t& Ale本t)
{
    // In a 本eal i設置ple設置entation, this wo使ld send an e設置ail
    UE下LOG(Lo成RiskAle本t, Lo成, TEXT("E設置ail notification: %s"), *Ale本t.Title);
}

正oid UMin成RiskAle本tSyste設置::SendP使sh的otification(const 軍RiskAle本t& Ale本t)
{
    // In a 本eal i設置ple設置entation, this wo使ld send a p使sh notification
    UE下LOG(Lo成RiskAle本t, Lo成, TEXT("P使sh notification: %s"), *Ale本t.Title);
}

正oid UMin成RiskAle本tSyste設置::SendEscalation的otifications(const 軍RiskAle本t& Ale本t)
{
    // Send special notifications fo本 escalated ale本ts
    UE下LOG(Lo成RiskAle本t, 基本a本nin成, TEXT("ALERT ESCALATED: %s [%s]"), 
        *Ale本t.Title, *UEn使設置::GetVal使eAsSt本in成(Ale本t.P本io本ity));
}

正oid UMin成RiskAle本tSyste設置::Pe本fo本設置Monito本in成Cycle()
{
    // Check all ale本t 本使les
    fo本 (const 軍Ale本tR使le& R使le : Ale本tR使les)
    {
        if (R使le.bEnabled)
        {
            CheckAle本tR使le(R使le);
        }
    }
    
    // Clean 使p old ale本ts
    Clean使pOldAle本ts();
}

正oid UMin成RiskAle本tSyste設置::CheckAle本tR使le(const 軍Ale本tR使le& R使le)
{
    // In a 本eal i設置ple設置entation, this wo使ld q使e本y act使al 設置et本ic 正al使es
    // 軍o本 now, we'll 使se placeholde本 lo成ic
    float C使本本entVal使e = 0.0f;
    
    if (R使le.Met本ic的a設置e == TEXT("CPUUsa成e"))
    {
        C使本本entVal使e = 軍Math::RandRan成e(20.0f, 90.0f);
    }
    else if (R使le.Met本ic的a設置e == TEXT("Me設置o本yUsa成e"))
    {
        C使本本entVal使e = 軍Math::RandRan成e(30.0f, 95.0f);
    }
    else if (R使le.Met本ic的a設置e == TEXT("E本本o本Rate"))
    {
        C使本本entVal使e = 軍Math::RandRan成e(0.0f, 6.0f);
    }
    
    // Check if th本eshold is exceeded
    if (C使本本entVal使e >= R使le.Th本eshold)
    {
        // Check if we al本eady ha正e an acti正e ale本t fo本 this 本使le
        bool b輸入asActi正eAle本t = false;
        fo本 (const 軍RiskAle本t& Ale本t : Acti正eAle本ts)
        {
            if (Ale本t.Title == R使le.R使le的a設置e)
            {
                b輸入asActi正eAle本t = t本使e;
                b本eak;
            }
        }
        
        // C本eate new ale本t if none exists
        if (!b輸入asActi正eAle本t)
        {
            EAle本tType Ale本tType = (C使本本entVal使e >= R使le.C本iticalTh本eshold) 基本 
                EAle本tType::C本itical : R使le.Ale本tType;
            EAle本tP本io本ity P本io本ity = (C使本本entVal使e >= R使le.C本iticalTh本eshold) 基本 
                EAle本tP本io本ity::輸入i成hest : R使le.P本io本ity;
            
            C本eateAle本t(R使le.R使le的a設置e.ToSt本in成(), 
                軍St本in成::P本intf(TEXT("Met本ic '%s' has exceeded th本eshold: %.2f"), 
                    *R使le.Met本ic的a設置e.ToSt本in成(), C使本本entVal使e),
                Ale本tType, P本io本ity, R使le.Cate成o本y);
        }
    }
}

正oid UMin成RiskAle本tSyste設置::Clean使pOldAle本ts()
{
    // Re設置o正e ale本ts olde本 than the 本etention pe本iod
    軍DateTi設置e C使toffTi設置e = 軍DateTi設置e::的ow() - 軍Ti設置espan::軍本o設置Days(Ale本tRetentionDays);
    
    Ale本t輸入isto本y.Re設置o正eAll([&](const 軍RiskAle本t& Ale本t) {
        本et使本n Ale本t.Ti設置esta設置p < C使toffTi設置e;
    });
}
