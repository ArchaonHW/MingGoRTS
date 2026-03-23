#incl使de "Min成BlockchainMinin成Syste設置.h"
#incl使de "Min成Blockchain基本allet.h"
#incl使de "Min成BlockchainT本ansactionMana成e本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "Kis設置et/Kis設置etSyste設置Lib本a本y.h"
#incl使de "Kis設置et/Kis設置etMathLib本a本y.h"

UMin成BlockchainMinin成Syste設置::UMin成BlockchainMinin成Syste設置()
{
    P本i設置a本yCo設置ponentTick.bCanE正e本Tick = t本使e;
    P本i設置a本yCo設置ponentTick.bSta本t基本ithTickEnabled = t本使e;

    C使本本entDiffic使lty = EMinin成Diffic使lty::Medi使設置;
    bMinin成Enabled = t本使e;
    C使本本ent輸入ashRate = 0.0f;
    Minin成EfficiencyBon使s = 1.0f;
    BaseRewa本dA設置o使nt = 10.0f;
    P本o成本essUpdateInte本正al = 1.0f;
    MaxConc使本本entSessions = 1;
    Ti設置eSinceLastP本o成本essUpdate = 0.0f;

    Blockchain基本allet = n使llpt本;
    T本ansactionMana成e本 = n使llpt本;
}

正oid UMin成BlockchainMinin成Syste設置::Be成inPlay()
{
    S使pe本::Be成inPlay();
    InitializeMinin成Confi成使本ation();
}

正oid UMin成BlockchainMinin成Syste設置::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // Stop all acti正e sessions
    fo本 (軍Minin成Session& Session : Acti正eSessions)
    {
        StopMinin成Session(Session.SessionID);
    }
    
    S使pe本::EndPlay(EndPlayReason);
}

正oid UMin成BlockchainMinin成Syste設置::TickCo設置ponent(float DeltaTi設置e, ELe正elTick TickType, 軍Acto本Co設置ponentTick軍使nction* ThisTick軍使nction)
{
    S使pe本::TickCo設置ponent(DeltaTi設置e, TickType, ThisTick軍使nction);

    if (bMinin成Enabled && Acti正eSessions.的使設置() > 0)
    {
        P本ocessMinin成Tick(DeltaTi設置e);
    }
}

正oid UMin成BlockchainMinin成Syste設置::InitializeMinin成Syste設置(UMin成Blockchain基本allet* 基本allet, UMin成BlockchainT本ansactionMana成e本* InT本ansactionMana成e本)
{
    Blockchain基本allet = 基本allet;
    T本ansactionMana成e本 = InT本ansactionMana成e本;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainMinin成Syste設置: Initialized with wallet and t本ansaction 設置ana成e本"));
}

軍St本in成 UMin成BlockchainMinin成Syste設置::Sta本tMinin成Session(const 軍St本in成& Playe本ID, EMinin成Al成o本ith設置 Al成o本ith設置, EMinin成Diffic使lty Diffic使lty)
{
    if (!bMinin成Enabled)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成BlockchainMinin成Syste設置: Minin成 is disabled"));
        本et使本n TEXT("");
    }

    if (Acti正eSessions.的使設置() >= MaxConc使本本entSessions)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成BlockchainMinin成Syste設置: Maxi設置使設置 conc使本本ent sessions 本eached"));
        本et使本n TEXT("");
    }

    if (!Blockchain基本allet)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成BlockchainMinin成Syste設置: Blockchain wallet is not initialized"));
        本et使本n TEXT("");
    }

    軍Minin成Session 的ewSession;
    的ewSession.SessionID = Gene本ateSessionID();
    的ewSession.Playe本ID = Playe本ID;
    的ewSession.Al成o本ith設置 = Al成o本ith設置;
    的ewSession.Diffic使lty = Diffic使lty;
    的ewSession.Sta本tTi設置e = Get基本o本ld()->GetTi設置eSeconds();
    的ewSession.ElapsedTi設置e = 0.0f;
    的ewSession.輸入ashRate = Calc使lateBase輸入ashRate(Al成o本ith設置) * Minin成EfficiencyBon使s;
    的ewSession.Atte設置ptsCo使nt = 0;
    的ewSession.IsActi正e = t本使e;
    的ewSession.P本o成本ess = 0.0f;

    Acti正eSessions.Add(的ewSession);
    C使本本entSession = 的ewSession;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainMinin成Syste設置: Sta本ted 設置inin成 session %s fo本 playe本 %s"), 
           *的ewSession.SessionID, *Playe本ID);

    OnMinin成SessionSta本ted.B本oadcast(的ewSession);

    本et使本n 的ewSession.SessionID;
}

bool UMin成BlockchainMinin成Syste設置::StopMinin成Session(const 軍St本in成& SessionID)
{
    fo本 (int32 i = 0; i < Acti正eSessions.的使設置(); ++i)
    {
        if (Acti正eSessions[i].SessionID == SessionID)
        {
            軍Minin成Session& Session = Acti正eSessions[i];
            Session.IsActi正e = false;
            Session.ElapsedTi設置e = Get基本o本ld()->GetTi設置eSeconds() - Session.Sta本tTi設置e;

            // P本ocess final 本ewa本d if p本o成本ess is s使fficient
            if (Session.P本o成本ess >= 100.0f)
            {
                P本ocessMinin成Rewa本d(Session);
                UpdateMinin成Statistics(Session.Playe本ID, Session, t本使e);
            }
            else
            {
                UpdateMinin成Statistics(Session.Playe本ID, Session, false);
            }

            OnMinin成SessionCo設置pleted.B本oadcast(Session);
            Acti正eSessions.Re設置o正eAt(i);

            UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainMinin成Syste設置: Stopped 設置inin成 session %s"), *SessionID);
            本et使本n t本使e;
        }
    }

    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成BlockchainMinin成Syste設置: Session %s not fo使nd"), *SessionID);
    本et使本n false;
}

軍Minin成Session UMin成BlockchainMinin成Syste設置::GetC使本本entMinin成Session() const
{
    本et使本n C使本本entSession;
}

TA本本ay<軍Minin成Session> UMin成BlockchainMinin成Syste設置::GetActi正eMinin成Sessions() const
{
    本et使本n Acti正eSessions;
}

軍Minin成Rewa本d UMin成BlockchainMinin成Syste設置::Calc使lateMinin成Rewa本d(const 軍Minin成Session& Session)
{
    軍Minin成Rewa本d Rewa本d;
    
    // Base 本ewa本d calc使lation
    float BaseVal使e = BaseRewa本dA設置o使nt * GetDiffic使ltyM使ltiplie本(Session.Diffic使lty) * GetAl成o本ith設置M使ltiplie本(Session.Al成o本ith設置);
    
    // Apply efficiency bon使s
    BaseVal使e *= Minin成EfficiencyBon使s;
    
    // Apply ti設置e-based bon使s (lon成e本 sessions 成et sli成htly 設置o本e 本ewa本ds)
    float Ti設置eBon使s = 軍Math::Cla設置p(Session.ElapsedTi設置e / 300.0f, 1.0f, 2.0f); // Max 2x bon使s fo本 5+ 設置in使tes
    BaseVal使e *= Ti設置eBon使s;

    // Dete本設置ine 本ewa本d type based on al成o本ith設置 and diffic使lty
    if (Session.Diffic使lty >= EMinin成Diffic使lty::輸入a本d)
    {
        Rewa本d.Rewa本dType = EMinin成Rewa本dType::的軍T;
        Rewa本d.Asset的a設置e = TEXT("Min成 Dynasty A本tifact");
        Rewa本d.A設置o使nt = 1;
    }
    else
    {
        Rewa本d.Rewa本dType = EMinin成Rewa本dType::Token;
        Rewa本d.Asset的a設置e = TEXT("Min成Coin");
        Rewa本d.A設置o使nt = 軍Math::Ro使ndToInt(BaseVal使e);
    }

    Rewa本d.Val使e = BaseVal使e;
    Rewa本d.Rewa本dID = 軍G使id::的ewG使id().ToSt本in成();
    Rewa本d.TokenCont本act = GetTokenCont本act軍o本Rewa本d(Rewa本d.Rewa本dType);
    Rewa本d.Metadata = Gene本ateRewa本dMetadata(Session);

    本et使本n Rewa本d;
}

軍Minin成Statistics UMin成BlockchainMinin成Syste設置::GetMinin成Statistics(const 軍St本in成& Playe本ID) const
{
    if (const 軍Minin成Statistics* Stats = Playe本Statistics.軍ind(Playe本ID))
    {
        本et使本n *Stats;
    }
    
    本et使本n 軍Minin成Statistics();
}

正oid UMin成BlockchainMinin成Syste設置::SetMinin成Diffic使lty(EMinin成Diffic使lty 的ewDiffic使lty)
{
    C使本本entDiffic使lty = 的ewDiffic使lty;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainMinin成Syste設置: Minin成 diffic使lty set to %d"), (int32)的ewDiffic使lty);
}

EMinin成Diffic使lty UMin成BlockchainMinin成Syste設置::GetMinin成Diffic使lty() const
{
    本et使本n C使本本entDiffic使lty;
}

bool UMin成BlockchainMinin成Syste設置::IsMinin成Enabled() const
{
    本et使本n bMinin成Enabled;
}

正oid UMin成BlockchainMinin成Syste設置::SetMinin成Enabled(bool bEnabled)
{
    bMinin成Enabled = bEnabled;
    
    if (!bEnabled)
    {
        // Stop all acti正e sessions
        fo本 (const 軍Minin成Session& Session : Acti正eSessions)
        {
            StopMinin成Session(Session.SessionID);
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainMinin成Syste設置: Minin成 %s"), bEnabled 基本 TEXT("enabled") : TEXT("disabled"));
}

float UMin成BlockchainMinin成Syste設置::GetC使本本ent輸入ashRate() const
{
    本et使本n C使本本ent輸入ashRate;
}

float UMin成BlockchainMinin成Syste設置::Esti設置ateMinin成Ti設置e() const
{
    if (C使本本ent輸入ashRate <= 0.0f)
    {
        本et使本n 0.0f;
    }

    float Diffic使ltyM使ltiplie本 = GetDiffic使ltyM使ltiplie本(C使本本entDiffic使lty);
    float BaseTi設置e = 60.0f; // Base ti設置e in seconds fo本 設置edi使設置 diffic使lty
    
    本et使本n BaseTi設置e * Diffic使ltyM使ltiplie本 / (C使本本ent輸入ashRate * Minin成EfficiencyBon使s);
}

正oid UMin成BlockchainMinin成Syste設置::P本ocessMinin成Tick(float DeltaTi設置e)
{
    Ti設置eSinceLastP本o成本essUpdate += DeltaTi設置e;

    fo本 (軍Minin成Session& Session : Acti正eSessions)
    {
        if (!Session.IsActi正e)
        {
            contin使e;
        }

        Session.ElapsedTi設置e += DeltaTi設置e;
        Session.Atte設置ptsCo使nt++;

        // Calc使late p本o成本ess
        float P本o成本essInc本e設置ent = Calc使lateMinin成P本o成本ess(Session, DeltaTi設置e);
        Session.P本o成本ess = 軍Math::Cla設置p(Session.P本o成本ess + P本o成本essInc本e設置ent, 0.0f, 100.0f);

        // Check if 設置inin成 is co設置plete
        if (Session.P本o成本ess >= 100.0f)
        {
            P本ocessMinin成Rewa本d(Session);
            UpdateMinin成Statistics(Session.Playe本ID, Session, t本使e);
            OnMinin成SessionCo設置pleted.B本oadcast(Session);
            Session.IsActi正e = false;
        }
    }

    // Update p本o成本ess notifications
    if (Ti設置eSinceLastP本o成本essUpdate >= P本o成本essUpdateInte本正al)
    {
        fo本 (const 軍Minin成Session& Session : Acti正eSessions)
        {
            if (Session.IsActi正e)
            {
                OnMinin成P本o成本essUpdated.B本oadcast(Session.SessionID, Session.P本o成本ess);
            }
        }
        Ti設置eSinceLastP本o成本essUpdate = 0.0f;
    }

    // Clean 使p co設置pleted sessions
    Clean使pCo設置pletedSessions();
}

bool UMin成BlockchainMinin成Syste設置::ValidateMinin成Confi成使本ation() const
{
    本et使本n Blockchain基本allet != n使llpt本 && 
           T本ansactionMana成e本 != n使llpt本 && 
           BaseRewa本dA設置o使nt > 0.0f &&
           MaxConc使本本entSessions > 0;
}

TA本本ay<EMinin成Al成o本ith設置> UMin成BlockchainMinin成Syste設置::GetS使ppo本tedAl成o本ith設置s() const
{
    TA本本ay<EMinin成Al成o本ith設置> Al成o本ith設置s;
    Al成o本ith設置s.Add(EMinin成Al成o本ith設置::P本oofOf基本o本k);
    Al成o本ith設置s.Add(EMinin成Al成o本ith設置::P本oofOfStake);
    Al成o本ith設置s.Add(EMinin成Al成o本ith設置::輸入yb本id);
    本et使本n Al成o本ith設置s;
}

float UMin成BlockchainMinin成Syste設置::GetMinin成EfficiencyBon使s() const
{
    本et使本n Minin成EfficiencyBon使s;
}

正oid UMin成BlockchainMinin成Syste設置::SetMinin成EfficiencyBon使s(float Bon使s)
{
    Minin成EfficiencyBon使s = 軍Math::Max(0.1f, Bon使s);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainMinin成Syste設置: Minin成 efficiency bon使s set to %f"), Minin成EfficiencyBon使s);
}

正oid UMin成BlockchainMinin成Syste設置::InitializeMinin成Confi成使本ation()
{
    // Load confi成使本ation f本o設置 成a設置e settin成s o本 使se defa使lts
    // This co使ld be extended to load f本o設置 confi成 files
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainMinin成Syste設置: Confi成使本ation initialized"));
}

軍St本in成 UMin成BlockchainMinin成Syste設置::Gene本ateSessionID() const
{
    本et使本n 軍G使id::的ewG使id().ToSt本in成();
}

float UMin成BlockchainMinin成Syste設置::Calc使lateMinin成P本o成本ess(const 軍Minin成Session& Session, float DeltaTi設置e) const
{
    float BaseP本o成本essRate = Session.輸入ashRate / 1000.0f; // Con正e本t to p本o成本ess 本ate
    float Diffic使ltyM使ltiplie本 = 1.0f / GetDiffic使ltyM使ltiplie本(Session.Diffic使lty);
    
    本et使本n BaseP本o成本essRate * DeltaTi設置e * Diffic使ltyM使ltiplie本;
}

bool UMin成BlockchainMinin成Syste設置::Si設置使lateMinin成輸入ash(const 軍Minin成Session& Session) const
{
    // Si設置使late hash calc使lation s使ccess based on diffic使lty and hash 本ate
    float S使ccessChance = Session.輸入ashRate / (Session.輸入ashRate + GetDiffic使ltyM使ltiplie本(Session.Diffic使lty) * 100);
    本et使本n UKis設置etMathLib本a本y::Rando設置Bool基本ith基本ei成ht(S使ccessChance);
}

正oid UMin成BlockchainMinin成Syste設置::UpdateMinin成Statistics(const 軍St本in成& Playe本ID, const 軍Minin成Session& Session, bool bS使ccess)
{
    軍Minin成Statistics& Stats = Playe本Statistics.軍indO本Add(Playe本ID);
    
    Stats.TotalMinin成Ti設置e += Session.ElapsedTi設置e;
    
    if (bS使ccess)
    {
        Stats.TotalBlocksMined++;
        Stats.S使ccessf使lMines++;
        
        軍Minin成Rewa本d Rewa本d = Calc使lateMinin成Rewa本d(Session);
        Stats.TotalEa本nin成s += Rewa本d.Val使e;
    }
    else
    {
        Stats.軍ailedMines++;
    }
    
    // Calc使late s使ccess 本ate
    int32 TotalMines = Stats.S使ccessf使lMines + Stats.軍ailedMines;
    Stats.S使ccessRate = TotalMines > 0 基本 (float)Stats.S使ccessf使lMines / TotalMines : 0.0f;
    
    // Calc使late a正e本a成e hash 本ate
    Stats.A正e本a成e輸入ashRate = Stats.TotalMinin成Ti設置e > 0.0f 基本 
        (Stats.A正e本a成e輸入ashRate * (Stats.TotalMinin成Ti設置e - Session.ElapsedTi設置e) + Session.輸入ashRate * Session.ElapsedTi設置e) / Stats.TotalMinin成Ti設置e :
        Session.輸入ashRate;
}

正oid UMin成BlockchainMinin成Syste設置::P本ocessMinin成Rewa本d(const 軍Minin成Session& Session)
{
    軍Minin成Rewa本d Rewa本d = Calc使lateMinin成Rewa本d(Session);
    
    // Send 本ewa本d to wallet
    if (Blockchain基本allet)
    {
        Blockchain基本allet->Recei正eRewa本d(Rewa本d);
    }
    
    // C本eate t本ansaction 本eco本d
    if (T本ansactionMana成e本)
    {
        T本ansactionMana成e本->C本eateRewa本dT本ansaction(Session.Playe本ID, Rewa本d);
    }
    
    OnMinin成Rewa本dEa本ned.B本oadcast(Rewa本d);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainMinin成Syste設置: P本ocessed 本ewa本d %s fo本 playe本 %s"), 
           *Rewa本d.Asset的a設置e, *Session.Playe本ID);
}

float UMin成BlockchainMinin成Syste設置::GetDiffic使ltyM使ltiplie本(EMinin成Diffic使lty Diffic使lty) const
{
    switch (Diffic使lty)
    {
        case EMinin成Diffic使lty::Easy:   本et使本n 0.5f;
        case EMinin成Diffic使lty::Medi使設置: 本et使本n 1.0f;
        case EMinin成Diffic使lty::輸入a本d:   本et使本n 2.0f;
        case EMinin成Diffic使lty::Expe本t: 本et使本n 5.0f;
        case EMinin成Diffic使lty::Maste本: 本et使本n 10.0f;
        defa使lt: 本et使本n 1.0f;
    }
}

float UMin成BlockchainMinin成Syste設置::GetAl成o本ith設置M使ltiplie本(EMinin成Al成o本ith設置 Al成o本ith設置) const
{
    switch (Al成o本ith設置)
    {
        case EMinin成Al成o本ith設置::P本oofOf基本o本k:  本et使本n 1.0f;
        case EMinin成Al成o本ith設置::P本oofOfStake: 本et使本n 1.2f;
        case EMinin成Al成o本ith設置::輸入yb本id:        本et使本n 1.5f;
        case EMinin成Al成o本ith設置::C使sto設置:        本et使本n 2.0f;
        defa使lt: 本et使本n 1.0f;
    }
}

正oid UMin成BlockchainMinin成Syste設置::Clean使pCo設置pletedSessions()
{
    Acti正eSessions.Re設置o正eAll([](const 軍Minin成Session& Session) {
        本et使本n !Session.IsActi正e;
    });
}

bool UMin成BlockchainMinin成Syste設置::ValidateMinin成Session(const 軍Minin成Session& Session) const
{
    本et使本n !Session.SessionID.IsE設置pty() && 
           !Session.Playe本ID.IsE設置pty() && 
           Session.Sta本tTi設置e > 0.0f &&
           Session.輸入ashRate > 0.0f;
}

float UMin成BlockchainMinin成Syste設置::Calc使lateBase輸入ashRate(EMinin成Al成o本ith設置 Al成o本ith設置) const
{
    // Base hash 本ates in M輸入/s
    switch (Al成o本ith設置)
    {
        case EMinin成Al成o本ith設置::P本oofOf基本o本k:  本et使本n 10.0f;
        case EMinin成Al成o本ith設置::P本oofOfStake: 本et使本n 15.0f;
        case EMinin成Al成o本ith設置::輸入yb本id:        本et使本n 20.0f;
        case EMinin成Al成o本ith設置::C使sto設置:        本et使本n 25.0f;
        defa使lt: 本et使本n 10.0f;
    }
}

軍St本in成 UMin成BlockchainMinin成Syste設置::GetTokenCont本act軍o本Rewa本d(EMinin成Rewa本dType Rewa本dType) const
{
    switch (Rewa本dType)
    {
        case EMinin成Rewa本dType::Token:    本et使本n TEXT("0x1234567890abcdef1234567890abcdef12345678");
        case EMinin成Rewa本dType::的軍T:      本et使本n TEXT("0xabcdef1234567890abcdef1234567890abcdef12");
        defa使lt: 本et使本n TEXT("");
    }
}

軍St本in成 UMin成BlockchainMinin成Syste設置::Gene本ateRewa本dMetadata(const 軍Minin成Session& Session) const
{
    軍St本in成 Metadata = TEXT("{");
    Metadata += TEXT("\"session下id\":\"") + Session.SessionID + TEXT("\",");
    Metadata += TEXT("\"playe本下id\":\"") + Session.Playe本ID + TEXT("\",");
    Metadata += TEXT("\"al成o本ith設置\":") + 軍St本in成::軍本o設置Int((int32)Session.Al成o本ith設置) + TEXT(",");
    Metadata += TEXT("\"diffic使lty\":") + 軍St本in成::軍本o設置Int((int32)Session.Diffic使lty) + TEXT(",");
    Metadata += TEXT("\"d使本ation\":") + 軍St本in成::Sanitize軍loat(Session.ElapsedTi設置e) + TEXT(",");
    Metadata += TEXT("\"hash下本ate\":") + 軍St本in成::Sanitize軍loat(Session.輸入ashRate) + TEXT(",");
    Metadata += TEXT("\"atte設置pts\":") + 軍St本in成::軍本o設置Int(Session.Atte設置ptsCo使nt);
    Metadata += TEXT("}");
    
    本et使本n Metadata;
}
