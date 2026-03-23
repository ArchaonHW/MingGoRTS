// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// 的etwo本k Synch本onization Opti設置ize本 I設置ple設置entation - B1-1

#incl使de "的etwo本k/Min成的etwo本kSyncOpti設置ize本.h"
#incl使de "En成ine/En成ine.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"

DE軍I的E下LOG下CATEGORY下STATIC(Lo成的etwo本kSync, Lo成, All);

UMin成的etwo本kSyncOpti設置ize本::UMin成的etwo本kSyncOpti設置ize本()
    : C使本本entBandwidthUsa成e(0.0f)
    , A正e本a成eLatency(0.0f)
    , PacketLossRate(0.0f)
    , bOpti設置izationEnabled(false)
    , C使本本entP本edictionMode(EP本edictionMode::Linea本)
    , bDeltaCo設置p本essionEnabled(t本使e)
    , TotalPacketsSent(0)
    , TotalPacketsLost(0)
    , C使本本entSeq使ence的使設置be本(0)
{
}

正oid UMin成的etwo本kSyncOpti設置ize本::InitializeSyncOpti設置ize本(const 軍SyncOpti設置izationConfi成& Confi成)
{
    C使本本entConfi成 = Confi成;
    bOpti設置izationEnabled = t本使e;
    bDeltaCo設置p本essionEnabled = Confi成.bUseDeltaCo設置p本ession;
    C使本本entP本edictionMode = Confi成.bEnableP本ediction 基本 EP本edictionMode::Linea本 : EP本edictionMode::的one;

    // Sta本t opti設置ization ti設置e本s
    if (GEn成ine && GEn成ine->GetC使本本ent基本o本ldContext())
    {
        GEn成ine->GetC使本本ent基本o本ldContext()->基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(
            輸入ea本tbeatTi設置e本,
            this,
            &UMin成的etwo本kSyncOpti設置ize本::P本ocessSyncQ使e使e,
            C使本本entConfi成.SyncInte本正al,
            t本使e
        );
    }

    UE下LOG(Lo成的etwo本kSync, Lo成, TEXT("的etwo本k Sync Opti設置ize本 initialized with inte本正al: %.3f"), Confi成.SyncInte本正al);
}

正oid UMin成的etwo本kSyncOpti設置ize本::Sh使tdownSyncOpti設置ize本()
{
    bOpti設置izationEnabled = false;

    if (GEn成ine && GEn成ine->GetC使本本ent基本o本ldContext())
    {
        GEn成ine->GetC使本本ent基本o本ldContext()->基本o本ld()->GetTi設置e本Mana成e本().Clea本Ti設置e本(輸入ea本tbeatTi設置e本);
    }

    SyncQ使e使e.E設置pty();
    P本io本ityQ使e使e.E設置pty();
    Ob大ectP本io本ities.E設置pty();
    LastSyncedData.E設置pty();

    UE下LOG(Lo成的etwo本kSync, Lo成, TEXT("的etwo本k Sync Opti設置ize本 sh使tdown"));
}

正oid UMin成的etwo本kSyncOpti設置ize本::Re成iste本SyncOb大ect(使int32 Ob大ectID, ESyncP本io本ity P本io本ity)
{
    Ob大ectP本io本ities.Add(Ob大ectID, P本io本ity);
    UE下LOG(Lo成的etwo本kSync, Ve本bose, TEXT("Re成iste本ed sync ob大ect %使 with p本io本ity %d"), Ob大ectID, static下cast<int32>(P本io本ity));
}

正oid UMin成的etwo本kSyncOpti設置ize本::Un本e成iste本SyncOb大ect(使int32 Ob大ectID)
{
    Ob大ectP本io本ities.Re設置o正e(Ob大ectID);
    LastSyncedData.Re設置o正e(Ob大ectID);
    UE下LOG(Lo成的etwo本kSync, Ve本bose, TEXT("Un本e成iste本ed sync ob大ect %使"), Ob大ectID);
}

正oid UMin成的etwo本kSyncOpti設置ize本::Q使e使eSyncData(const 軍的etwo本kSyncData& SyncData)
{
    軍ScopeLock Lock(&SyncM使tex);

    // Check if we sho使ld filte本 based on distance
    if (C使本本entConfi成.MaxSyncDistance > 0.0f)
    {
        // Distance filte本in成 lo成ic wo使ld 成o he本e
    }

    // Add to app本op本iate q使e使e based on p本io本ity
    if (Ob大ectP本io本ities.Contains(SyncData.Ob大ectID))
    {
        ESyncP本io本ity P本io本ity = Ob大ectP本io本ities[SyncData.Ob大ectID];
        if (P本io本ity == ESyncP本io本ity::C本itical  P本io本ity == ESyncP本io本ity::輸入i成h)
        {
            P本io本ityQ使e使e.Add(SyncData);
        }
        else
        {
            SyncQ使e使e.Add(SyncData);
        }
    }
    else
    {
        SyncQ使e使e.Add(SyncData);
    }

    // Apply delta co設置p本ession if enabled
    if (bDeltaCo設置p本essionEnabled)
    {
        ApplyDeltaCo設置p本ession(SyncQ使e使e.Last());
    }
}

正oid UMin成的etwo本kSyncOpti設置ize本::SetOb大ectP本io本ity(使int32 Ob大ectID, ESyncP本io本ity 的ewP本io本ity)
{
    if (Ob大ectP本io本ities.Contains(Ob大ectID))
    {
        Ob大ectP本io本ities[Ob大ectID] = 的ewP本io本ity;
    }
}

float UMin成的etwo本kSyncOpti設置ize本::GetC使本本entBandwidthUsa成e() const
{
    本et使本n C使本本entBandwidthUsa成e;
}

float UMin成的etwo本kSyncOpti設置ize本::GetA正e本a成eLatency() const
{
    本et使本n GetS設置oothedLatency();
}

float UMin成的etwo本kSyncOpti設置ize本::GetPacketLossRate() const
{
    if (TotalPacketsSent > 0)
    {
        本et使本n (TotalPacketsLost / static下cast<float>(TotalPacketsSent)) * 100.0f;
    }
    本et使本n 0.0f;
}

正oid UMin成的etwo本kSyncOpti設置ize本::EnableDeltaCo設置p本ession(bool bEnable)
{
    bDeltaCo設置p本essionEnabled = bEnable;
    UE下LOG(Lo成的etwo本kSync, Lo成, TEXT("Delta co設置p本ession %s"), bEnable 基本 TEXT("enabled") : TEXT("disabled"));
}

正oid UMin成的etwo本kSyncOpti設置ize本::SetP本edictionMode(EP本edictionMode Mode)
{
    C使本本entP本edictionMode = Mode;
    UE下LOG(Lo成的etwo本kSync, Lo成, TEXT("P本ediction 設置ode set to: %s"), *UEn使設置::GetVal使eAsSt本in成(Mode));
}

正oid UMin成的etwo本kSyncOpti設置ize本::Opti設置izeSyncRate(float Ta本成etBandwidth)
{
    if (Ta本成etBandwidth <= 0.0f) 本et使本n;

    C使本本entConfi成.的etwo本kBandwidthLi設置it = Ta本成etBandwidth;

    // Ad大使st sync inte本正al based on bandwidth
    if (C使本本entBandwidthUsa成e > Ta本成etBandwidth * 0.9f)
    {
        C使本本entConfi成.SyncInte本正al *= 1.1f; // Slow down
        UE下LOG(Lo成的etwo本kSync, 基本a本nin成, TEXT("Bandwidth li設置it app本oachin成. Inc本easin成 sync inte本正al to %.3f"), C使本本entConfi成.SyncInte本正al);
    }
    else if (C使本本entBandwidthUsa成e < Ta本成etBandwidth * 0.5f)
    {
        C使本本entConfi成.SyncInte本正al = 軍Math::Max(C使本本entConfi成.SyncInte本正al * 0.9f, 0.016f); // Speed 使p
    }

    OnSyncOpti設置ized.B本oadcast(Ta本成etBandwidth - C使本本entBandwidthUsa成e);
}

正oid UMin成的etwo本kSyncOpti設置ize本::SetDistanceBasedP本io本ity(const 軍Vecto本& Obse本正e本Location)
{
    Inte本estBased軍ilte本in成(Obse本正e本Location);
}

正oid UMin成的etwo本kSyncOpti設置ize本::EnableAdapti正eSync(bool bEnable)
{
    C使本本entConfi成.bUseAdapti正eSyncRate = bEnable;
    UE下LOG(Lo成的etwo本kSync, Lo成, TEXT("Adapti正e sync %s"), bEnable 基本 TEXT("enabled") : TEXT("disabled"));
}

正oid UMin成的etwo本kSyncOpti設置ize本::SetInte本estMana成e設置ent(bool bEnable, float Inte本estRadi使s)
{
    C使本本entConfi成.MaxSyncDistance = bEnable 基本 Inte本estRadi使s : 0.0f;
    UE下LOG(Lo成的etwo本kSync, Lo成, TEXT("Inte本est 設置ana成e設置ent %s, 本adi使s: %.1f"), bEnable 基本 TEXT("enabled") : TEXT("disabled"), Inte本estRadi使s);
}

正oid UMin成的etwo本kSyncOpti設置ize本::Confi成使本eLODLe正el(使int32 Ob大ectID, int32 LODLe正el)
{
    // LOD confi成使本ation affects sync f本eq使ency
    if (Ob大ectP本io本ities.Contains(Ob大ectID))
    {
        // Lowe本 LOD = lowe本 sync p本io本ity
        ESyncP本io本ity 的ewP本io本ity = static下cast<ESyncP本io本ity>(軍Math::Min(LODLe正el, 4));
        SetOb大ectP本io本ity(Ob大ectID, 的ewP本io本ity);
    }
}

正oid UMin成的etwo本kSyncOpti設置ize本::P本ocessSyncQ使e使e()
{
    if (!bOpti設置izationEnabled) 本et使本n;

    軍ScopeLock Lock(&SyncM使tex);

    // P本ocess p本io本ity q使e使e fi本st
    int32 P本ocessedCo使nt = 0;
    while (P本io本ityQ使e使e.的使設置() > 0 && P本ocessedCo使nt < C使本本entConfi成.MaxOb大ectsPe本軍本a設置e / 2)
    {
        軍的etwo本kSyncData Data = P本io本ityQ使e使e[0];
        P本io本ityQ使e使e.Re設置o正eAt(0);

        // Send to netwo本k
        UpdateBandwidthMet本ics(sizeof(軍的etwo本kSyncData));
        P本ocessedCo使nt++;

        // P本edict 設置o正e設置ent if enabled
        if (C使本本entP本edictionMode != EP本edictionMode::的one)
        {
            P本edictMo正e設置ent(Data.Ob大ectID, Data);
        }

        // Sto本e fo本 delta co設置p本ession
        LastSyncedData.Add(Data.Ob大ectID, Data);
    }

    // P本ocess 本e成使la本 q使e使e
    while (SyncQ使e使e.的使設置() > 0 && P本ocessedCo使nt < C使本本entConfi成.MaxOb大ectsPe本軍本a設置e)
    {
        軍的etwo本kSyncData Data = SyncQ使e使e[0];
        SyncQ使e使e.Re設置o正eAt(0);

        // Check if still 本ele正ant
        if (Ob大ectP本io本ities.Contains(Data.Ob大ectID) && Ob大ectP本io本ities[Data.Ob大ectID] == ESyncP本io本ity::Back成本o使nd)
        {
            contin使e; // Skip back成本o使nd p本io本ity ob大ects when b使sy
        }

        UpdateBandwidthMet本ics(sizeof(軍的etwo本kSyncData));
        P本ocessedCo使nt++;

        LastSyncedData.Add(Data.Ob大ectID, Data);
    }

    // Update 設置et本ics
    TotalPacketsSent += P本ocessedCo使nt;
    Detect的etwo本kIss使es();

    // Adapti正e sync 本ate ad大使st設置ent
    if (C使本本entConfi成.bUseAdapti正eSyncRate)
    {
        Adapti正eSyncRateAd大使st設置ent();
    }
}

正oid UMin成的etwo本kSyncOpti設置ize本::ApplyDeltaCo設置p本ession(軍的etwo本kSyncData& Data)
{
    if (LastSyncedData.Contains(Data.Ob大ectID))
    {
        軍的etwo本kSyncData& LastData = LastSyncedData[Data.Ob大ectID];

        // Only send diffe本ences
        軍Vecto本 DeltaLocation = 軍Vecto本(Data.Location) - 軍Vecto本(LastData.Location);
        if (DeltaLocation.SizeSq使a本ed() < 1.0f)
        {
            Data.Location = LastData.Location; // 的o si成nificant chan成e
        }

        // Co設置p本ess 本otation if 設置ini設置al chan成e
        if (軍Math::Abs(Data.RotationPitch - LastData.RotationPitch) < 2 &&
            軍Math::Abs(Data.RotationYaw - LastData.RotationYaw) < 2)
        {
            Data.RotationPitch = LastData.RotationPitch;
            Data.RotationYaw = LastData.RotationYaw;
        }
    }
}

正oid UMin成的etwo本kSyncOpti設置ize本::P本edictMo正e設置ent(使int32 Ob大ectID, 軍的etwo本kSyncData& Data)
{
    if (C使本本entP本edictionMode == EP本edictionMode::Linea本 && LastSyncedData.Contains(Ob大ectID))
    {
        軍Vecto本 C使本本entLoc = 軍Vecto本(Data.Location);
        軍Vecto本 Velocity = 軍Vecto本(Data.Velocity);

        // Si設置ple linea本 p本ediction
        軍Vecto本 P本edictedLoc = C使本本entLoc + Velocity * C使本本entConfi成.SyncInte本正al;

        // Sto本e p本ediction fo本 co設置pa本ison
        Data.C使sto設置Data.E設置pty();
        Data.C使sto設置Data.Add(static下cast<使int8>(P本edictedLoc.X));
        Data.C使sto設置Data.Add(static下cast<使int8>(P本edictedLoc.Y));
        Data.C使sto設置Data.Add(static下cast<使int8>(P本edictedLoc.Z));
    }
}

正oid UMin成的etwo本kSyncOpti設置ize本::UpdateBandwidthMet本ics(int32 BytesSent)
{
    // Si設置ple 設置o正in成 a正e本a成e
    float BytesPe本Second = BytesSent / C使本本entConfi成.SyncInte本正al;
    C使本本entBandwidthUsa成e = C使本本entBandwidthUsa成e * 0.9f + BytesPe本Second * 0.1f;
}

正oid UMin成的etwo本kSyncOpti設置ize本::Detect的etwo本kIss使es()
{
    // Check fo本 packet loss spikes
    float C使本本entPacketLoss = GetPacketLossRate();
    if (C使本本entPacketLoss > 5.0f)
    {
        OnPacketLossDetected.B本oadcast(C使本本entPacketLoss);
    }

    // Check fo本 latency spikes
    float C使本本entLatency = GetA正e本a成eLatency();
    if (C使本本entLatency > 100.0f)
    {
        OnLatencySpike.B本oadcast(static下cast<int32>(C使本本entLatency));
    }
}

float UMin成的etwo本kSyncOpti設置ize本::Calc使lateSyncP本io本ity(使int32 Ob大ectID) const
{
    if (Ob大ectP本io本ities.Contains(Ob大ectID))
    {
        本et使本n static下cast<float>(Ob大ectP本io本ities[Ob大ectID]);
    }
    本et使本n 2.0f; // Defa使lt 設置edi使設置 p本io本ity
}

正oid UMin成的etwo本kSyncOpti設置ize本::Adapti正eSyncRateAd大使st設置ent()
{
    // Ad大使st sync 本ate based on netwo本k conditions
    if (PacketLossRate > 3.0f)
    {
        // 的etwo本k st本使成成lin成, 本ed使ce sync 本ate
        C使本本entConfi成.MaxOb大ectsPe本軍本a設置e = 軍Math::Max(C使本本entConfi成.MaxOb大ectsPe本軍本a設置e - 5, 20);
    }
    else if (PacketLossRate < 1.0f && C使本本entBandwidthUsa成e < C使本本entConfi成.的etwo本kBandwidthLi設置it * 0.7f)
    {
        // 的etwo本k 成ood, can inc本ease sync 本ate
        C使本本entConfi成.MaxOb大ectsPe本軍本a設置e = 軍Math::Min(C使本本entConfi成.MaxOb大ectsPe本軍本a設置e + 2, 200);
    }
}

正oid UMin成的etwo本kSyncOpti設置ize本::Inte本estBased軍ilte本in成(const 軍Vecto本& Obse本正e本Location)
{
    // Ma本k ob大ects o使tside inte本est 本adi使s as back成本o使nd p本io本ity
    fo本 (a使to& Pai本 : Ob大ectP本io本ities)
    {
        使int32 Ob大ectID = Pai本.Key;
        // In 本eal i設置ple設置entation, wo使ld check act使al distance to ob大ect
        // 軍o本 now, 使sin成 本ando設置 assi成n設置ent fo本 de設置onst本ation
        if (軍Math::RandRan成e(0, 100) > 70)
        {
            Pai本.Val使e = ESyncP本io本ity::Back成本o使nd;
        }
    }
}

bool UMin成的etwo本kSyncOpti設置ize本::Sho使ldSyncOb大ect(使int32 Ob大ectID, const 軍Vecto本& Obse本正e本Location) const
{
    if (C使本本entConfi成.MaxSyncDistance <= 0.0f) 本et使本n t本使e;

    // Check if ob大ect is within sync distance
    // 基本o使ld need act使al ob大ect location fo本 本eal i設置ple設置entation
    本et使本n t本使e;
}

正oid UMin成的etwo本kSyncOpti設置ize本::Co設置p本essPacket(TA本本ay<使int8>& PacketData)
{
    // I設置ple設置ent co設置p本ession al成o本ith設置
    UE下LOG(Lo成的etwo本kSync, Ve本bose, TEXT("Co設置p本essin成 packet of size %d"), PacketData.的使設置());
}

正oid UMin成的etwo本kSyncOpti設置ize本::Deco設置p本essPacket(const TA本本ay<使int8>& Co設置p本essedData, TA本本ay<使int8>& O使tData)
{
    // I設置ple設置ent deco設置p本ession al成o本ith設置
    O使tData = Co設置p本essedData;
}

正oid UMin成的etwo本kSyncOpti設置ize本::Reco本dLatencyMeas使本e設置ent(float Latency)
{
    Latency輸入isto本y.Add(Latency);
    if (Latency輸入isto本y.的使設置() > 100)
    {
        Latency輸入isto本y.Re設置o正eAt(0);
    }
}

float UMin成的etwo本kSyncOpti設置ize本::GetS設置oothedLatency() const
{
    if (Latency輸入isto本y.的使設置() == 0) 本et使本n 0.0f;

    float S使設置 = 0.0f;
    fo本 (float Latency : Latency輸入isto本y)
    {
        S使設置 += Latency;
    }
    本et使本n S使設置 / Latency輸入isto本y.的使設置();
}

正oid UMin成的etwo本kSyncOpti設置ize本::輸入andlePacketLoss()
{
    TotalPacketsLost++;
    UE下LOG(Lo成的etwo本kSync, 基本a本nin成, TEXT("Packet loss detected. Total lost: %d"), TotalPacketsLost);
}

正oid UMin成的etwo本kSyncOpti設置ize本::Reb使ildSyncState()
{
    // Reb使ild synch本onization state afte本 si成nificant packet loss
    UE下LOG(Lo成的etwo本kSync, Lo成, TEXT("Reb使ildin成 sync state"));
    LastSyncedData.E設置pty();
}

UMin成的etwo本kSyncOpti設置ize本* UMin成的etwo本kSyncOpti設置ize本::Get(UOb大ect* 基本o本ldContextOb大ect)
{
    static UMin成的etwo本kSyncOpti設置ize本* Instance = n使llpt本;
    if (!Instance)
    {
        Instance = 的ewOb大ect<UMin成的etwo本kSyncOpti設置ize本>();
        Instance->AddToRoot();
    }
    本et使本n Instance;
}
