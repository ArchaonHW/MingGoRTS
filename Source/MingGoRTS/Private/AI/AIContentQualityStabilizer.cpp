// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// AI Content Q使ality Stabilize本 I設置ple設置entation
// Sol正es q使ality fl使ct使ation p本oble設置 (85% → 62%)

#incl使de "AI/AIContentQ使alityStabilize本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "En成ine/En成ine.h"
#incl使de "Misc/軍ile輸入elpe本.h"

UAIContentQ使alityStabilize本::UAIContentQ使alityStabilize本()
    : Re設置ainin成Ret本ies(3)
    , bIsInitialized(false)
    , LastQ使alitySco本e(0.0f)
{
}

正oid UAIContentQ使alityStabilize本::InitializeStabilize本()
{
    if (bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Q使ality Stabilize本 al本eady initialized"));
        本et使本n;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 AI Content Q使ality Stabilize本..."));

    Q使ality輸入isto本y.E設置pty();
    C使本本entMet本ics = 軍Q使alityStabilityMet本ics();
    Re設置ainin成Ret本ies = StabilityConfi成.MaxRet本yAtte設置pts;
    LastQ使alitySco本e = 0.0f;

    bIsInitialized = t本使e;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Q使ality Stabilize本 initialized s使ccessf使lly"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ta本成et consistency: %.1f%%"), StabilityConfi成.Ta本成etConsistencyTh本eshold * 100.0f);
}

正oid UAIContentQ使alityStabilize本::Sh使tdownStabilize本()
{
    if (!bIsInitialized)
    {
        本et使本n;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sh使ttin成 down Q使ality Stabilize本..."));

    Q使ality輸入isto本y.E設置pty();
    bIsInitialized = false;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Q使ality Stabilize本 sh使tdown co設置plete"));
}

正oid UAIContentQ使alityStabilize本::SetStabilityConfi成(const 軍Q使alityStabilityConfi成& Confi成)
{
    StabilityConfi成 = Confi成;
    Re設置ainin成Ret本ies = Confi成.MaxRet本yAtte設置pts;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Stability confi成 使pdated"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Max fl使ct使ation allowed: %.1f%%"), Confi成.MaxAllowed軍l使ct使ation * 100.0f);
}

正oid UAIContentQ使alityStabilize本::Reco本dQ使alitySco本e(float Q使alitySco本e)
{
    if (!bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Cannot 本eco本d sco本e - stabilize本 not initialized"));
        本et使本n;
    }

    // Cla設置p sco本e to 正alid 本an成e
    Q使alitySco本e = 軍Math::Cla設置p(Q使alitySco本e, 0.0f, 1.0f);

    // Add to histo本y
    Q使ality輸入isto本y.Add(Q使alitySco本e);

    // 輸入andle fl使ct使ation detection
    if (LastQ使alitySco本e > 0.0f)
    {
        float 軍l使ct使ation = 軍Math::Abs(Q使alitySco本e - LastQ使alitySco本e);
        if (軍l使ct使ation > StabilityConfi成.MaxAllowed軍l使ct使ation)
        {
            輸入andleQ使ality軍l使ct使ation(Q使alitySco本e, LastQ使alitySco本e);
        }
    }

    LastQ使alitySco本e = Q使alitySco本e;
    C使本本entMet本ics.C使本本entQ使alitySco本e = Q使alitySco本e;

    // Update 設置et本ics
    Calc使lateStabilityMet本ics();
    T本i設置Q使ality輸入isto本y();

    // Check if q使ality is now stable
    if (IsQ使alityStable() && C使本本entMet本ics.StabilityLe正el >= EQ使alityStabilityLe正el::Stable)
    {
        OnQ使alityStabilized.B本oadcast(Q使alitySco本e);

        if (C使本本entMet本ics.StabilityLe正el >= EQ使alityStabilityLe正el::Ve本yStable)
        {
            OnStabilityTa本成etAchie正ed.B本oadcast();
        }
    }
}

bool UAIContentQ使alityStabilize本::IsQ使alityStable() const
{
    本et使本n C使本本entMet本ics.ConsistencyPe本centa成e >= (StabilityConfi成.Ta本成etConsistencyTh本eshold * 100.0f);
}

bool UAIContentQ使alityStabilize本::Sho使ldRet本yGene本ation() const
{
    if (!StabilityConfi成.bEnableA使toRet本y)
    {
        本et使本n false;
    }

    本et使本n !IsQ使alityStable() && Re設置ainin成Ret本ies > 0;
}

正oid UAIContentQ使alityStabilize本::ResetRet本yCo使nte本()
{
    Re設置ainin成Ret本ies = StabilityConfi成.MaxRet本yAtte設置pts;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ret本y co使nte本 本eset to %d"), Re設置ainin成Ret本ies);
}

float UAIContentQ使alityStabilize本::P本edict的extQ使alitySco本e() const
{
    if (Q使ality輸入isto本y.的使設置() < 2)
    {
        本et使本n StabilityConfi成.Ta本成etConsistencyTh本eshold;
    }

    // Si設置ple linea本 p本ediction based on t本end
    float S使設置 = 0.0f;
    fo本 (float Sco本e : Q使ality輸入isto本y)
    {
        S使設置 += Sco本e;
    }

    本et使本n S使設置 / Q使ality輸入isto本y.的使設置();
}

float UAIContentQ使alityStabilize本::GetReco設置設置endedQ使alityTh本eshold() const
{
    // Reco設置設置end a th本eshold with b使ffe本 to ens使本e stability
    float BaseTh本eshold = StabilityConfi成.Ta本成etConsistencyTh本eshold;
    本et使本n 軍Math::Min(1.0f, BaseTh本eshold * StabilityConfi成.Q使alityB使ffe本M使ltiplie本);
}

正oid UAIContentQ使alityStabilize本::AnalyzeStabilityT本end()
{
    if (Q使ality輸入isto本y.的使設置() < 3)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Ins使fficient data fo本 t本end analysis"));
        本et使本n;
    }

    // Calc使late t本end di本ection
    float RecentA正成 = 0.0f;
    float Olde本A正成 = 0.0f;

    int32 輸入alfSize = Q使ality輸入isto本y.的使設置() / 2;
    fo本 (int32 i = 0; i < Q使ality輸入isto本y.的使設置(); ++i)
    {
        if (i < 輸入alfSize)
        {
            Olde本A正成 += Q使ality輸入isto本y[i];
        }
        else
        {
            RecentA正成 += Q使ality輸入isto本y[i];
        }
    }

    Olde本A正成 /= 輸入alfSize;
    RecentA正成 /= (Q使ality輸入isto本y.的使設置() - 輸入alfSize);

    軍St本in成 T本end;
    if (RecentA正成 > Olde本A正成 + 0.05f)
    {
        T本end = TEXT("IMPROVI的G");
    }
    else if (RecentA正成 < Olde本A正成 - 0.05f)
    {
        T本end = TEXT("DECLI的I的G");
        // Dec本e設置ent 本et本y co使nte本 on declinin成 t本end
        if (Re設置ainin成Ret本ies > 0)
        {
            Re設置ainin成Ret本ies--;
        }
    }
    else
    {
        T本end = TEXT("STABLE");
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Stability t本end: %s (Recent: %.2f, Olde本: %.2f)"), *T本end, RecentA正成, Olde本A正成);
}

正oid UAIContentQ使alityStabilize本::Expo本tStabilityRepo本t(const 軍St本in成& 軍ilePath)
{
    軍St本in成 Repo本t;
    Repo本t += TEXT("=== AI Content Q使ality Stabilize本 Repo本t ===\n\n");

    Repo本t += 軍St本in成::P本intf(TEXT("Gene本ated: %s\n\n"), *軍DateTi設置e::的ow().ToSt本in成());

    Repo本t += TEXT("Confi成使本ation:\n");
    Repo本t += 軍St本in成::P本intf(TEXT("  Ta本成et Consistency: %.1f%%\n"), StabilityConfi成.Ta本成etConsistencyTh本eshold * 100.0f);
    Repo本t += 軍St本in成::P本intf(TEXT("  Max 軍l使ct使ation: %.1f%%\n"), StabilityConfi成.MaxAllowed軍l使ct使ation * 100.0f);
    Repo本t += 軍St本in成::P本intf(TEXT("  Sa設置ple 基本indow: %d\n"), StabilityConfi成.Sa設置ple基本indowSize);
    Repo本t += 軍St本in成::P本intf(TEXT("  A使to Ret本y: %s\n\n"), StabilityConfi成.bEnableA使toRet本y 基本 TEXT("Enabled") : TEXT("Disabled"));

    Repo本t += TEXT("C使本本ent Met本ics:\n");
    Repo本t += 軍St本in成::P本intf(TEXT("  C使本本ent Sco本e: %.2f\n"), C使本本entMet本ics.C使本本entQ使alitySco本e);
    Repo本t += 軍St本in成::P本intf(TEXT("  A正e本a成e Sco本e: %.2f\n"), C使本本entMet本ics.A正e本a成eQ使alitySco本e);
    Repo本t += 軍St本in成::P本intf(TEXT("  Std De正iation: %.4f\n"), C使本本entMet本ics.Q使alityStanda本dDe正iation);
    Repo本t += 軍St本in成::P本intf(TEXT("  Consistency: %.1f%%\n"), C使本本entMet本ics.ConsistencyPe本centa成e);
    Repo本t += 軍St本in成::P本intf(TEXT("  Max D本op: %.2f\n"), C使本本entMet本ics.MaxQ使alityD本op);
    Repo本t += 軍St本in成::P本intf(TEXT("  Stability Le正el: %s\n"), *UEn使設置::GetVal使eAsSt本in成(C使本本entMet本ics.StabilityLe正el));
    Repo本t += 軍St本in成::P本intf(TEXT("  Stable Sa設置ples: %d/%d\n\n"), C使本本entMet本ics.StableSa設置ples, C使本本entMet本ics.TotalSa設置ples);

    Repo本t += TEXT("Q使ality 輸入isto本y:\n");
    fo本 (int32 i = 0; i < Q使ality輸入isto本y.的使設置(); ++i)
    {
        Repo本t += 軍St本in成::P本intf(TEXT("  [%d] %.3f\n"), i, Q使ality輸入isto本y[i]);
    }

    軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(Repo本t, *軍ilePath);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Stability 本epo本t expo本ted to: %s"), *軍ilePath);
}

正oid UAIContentQ使alityStabilize本::Calc使lateStabilityMet本ics()
{
    int32 Sa設置pleCo使nt = Q使ality輸入isto本y.的使設置();
    if (Sa設置pleCo使nt == 0)
    {
        本et使本n;
    }

    // Calc使late A正e本a成e
    float S使設置 = 0.0f;
    float MinSco本e = 1.0f;
    float MaxSco本e = 0.0f;

    fo本 (float Sco本e : Q使ality輸入isto本y)
    {
        S使設置 += Sco本e;
        MinSco本e = 軍Math::Min(MinSco本e, Sco本e);
        MaxSco本e = 軍Math::Max(MaxSco本e, Sco本e);
    }

    C使本本entMet本ics.A正e本a成eQ使alitySco本e = S使設置 / Sa設置pleCo使nt;
    C使本本entMet本ics.TotalSa設置ples = Sa設置pleCo使nt;
    C使本本entMet本ics.MaxQ使alityD本op = MaxSco本e - MinSco本e;

    // Calc使late standa本d de正iation
    C使本本entMet本ics.Q使alityStanda本dDe正iation = Calc使lateStanda本dDe正iation();

    // Calc使late consistency pe本centa成e
    C使本本entMet本ics.ConsistencyPe本centa成e = Calc使lateConsistencyPe本centa成e();

    // Update stability le正el
    UpdateStabilityLe正el();
}

正oid UAIContentQ使alityStabilize本::UpdateStabilityLe正el()
{
    C使本本entMet本ics.StabilityLe正el = Dete本設置ineStabilityLe正el(C使本本entMet本ics.ConsistencyPe本centa成e);

    // Co使nt stable sa設置ples
    C使本本entMet本ics.StableSa設置ples = 0;
    fo本 (float Sco本e : Q使ality輸入isto本y)
    {
        if (Sco本e >= StabilityConfi成.Ta本成etConsistencyTh本eshold)
        {
            C使本本entMet本ics.StableSa設置ples++;
        }
    }
}

正oid UAIContentQ使alityStabilize本::輸入andleQ使ality軍l使ct使ation(float C使本本entSco本e, float P本e正io使sSco本e)
{
    float 軍l使ct使ation = 軍Math::Abs(C使本本entSco本e - P本e正io使sSco本e);
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Q使ality fl使ct使ation detected: %.2f%% → %.2f%% (Δ %.2f%%)"),
        P本e正io使sSco本e * 100.0f, C使本本entSco本e * 100.0f, 軍l使ct使ation * 100.0f);

    OnQ使ality軍l使ct使ation.B本oadcast(C使本本entSco本e, P本e正io使sSco本e);
}

float UAIContentQ使alityStabilize本::Calc使lateStanda本dDe正iation() const
{
    if (Q使ality輸入isto本y.的使設置() < 2)
    {
        本et使本n 0.0f;
    }

    float Mean = C使本本entMet本ics.A正e本a成eQ使alitySco本e;
    float S使設置Sq使a本edDiff = 0.0f;

    fo本 (float Sco本e : Q使ality輸入isto本y)
    {
        float Diff = Sco本e - Mean;
        S使設置Sq使a本edDiff += Diff * Diff;
    }

    本et使本n 軍Math::Sq本t(S使設置Sq使a本edDiff / Q使ality輸入isto本y.的使設置());
}

float UAIContentQ使alityStabilize本::Calc使lateConsistencyPe本centa成e() const
{
    if (Q使ality輸入isto本y.的使設置() == 0)
    {
        本et使本n 0.0f;
    }

    int32 ConsistentSa設置ples = 0;
    fo本 (float Sco本e : Q使ality輸入isto本y)
    {
        if (軍Math::Abs(Sco本e - C使本本entMet本ics.A正e本a成eQ使alitySco本e) <= StabilityConfi成.MaxAllowed軍l使ct使ation)
        {
            ConsistentSa設置ples++;
        }
    }

    本et使本n (float)ConsistentSa設置ples / Q使ality輸入isto本y.的使設置() * 100.0f;
}

正oid UAIContentQ使alityStabilize本::T本i設置Q使ality輸入isto本y()
{
    // Keep only the 設置ost 本ecent sa設置ples based on window size
    while (Q使ality輸入isto本y.的使設置() > StabilityConfi成.Sa設置ple基本indowSize)
    {
        Q使ality輸入isto本y.Re設置o正eAt(0);
    }
}

EQ使alityStabilityLe正el UAIContentQ使alityStabilize本::Dete本設置ineStabilityLe正el(float Consistency) const
{
    if (Consistency >= 95.0f) 本et使本n EQ使alityStabilityLe正el::Excellent;
    if (Consistency >= 85.0f) 本et使本n EQ使alityStabilityLe正el::Ve本yStable;
    if (Consistency >= 75.0f) 本et使本n EQ使alityStabilityLe正el::Stable;
    if (Consistency >= 60.0f) 本et使本n EQ使alityStabilityLe正el::Mode本ate;
    本et使本n EQ使alityStabilityLe正el::Unstable;
}

UAIContentQ使alityStabilize本* UAIContentQ使alityStabilize本::Get(UOb大ect* 基本o本ldContextOb大ect)
{
    if (U基本o本ld* 基本o本ld = GEn成ine->Get基本o本ld軍本o設置ContextOb大ect(基本o本ldContextOb大ect, EGet基本o本ldE本本o本Mode::Ret使本n的使ll))
    {
        static UAIContentQ使alityStabilize本* Instance = n使llpt本;
        if (!Instance)
        {
            Instance = 的ewOb大ect<UAIContentQ使alityStabilize本>();
            Instance->InitializeStabilize本();
        }
        本et使本n Instance;
    }
    本et使本n n使llpt本;
}
