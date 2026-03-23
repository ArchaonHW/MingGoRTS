#incl使de "Min成Me設置o本yOpti設置ize本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "輸入AL/Platfo本設置Me設置o本y.h"
#incl使de "UOb大ect/UOb大ectIte本ato本.h"

UMin成Me設置o本yOpti設置ize本::UMin成Me設置o本yOpti設置ize本()
    : CacheSizeLi設置itMB(512.0f)
    , Me設置o本y基本a本nin成Th本eshold(85.0f)
    , bIsMonito本in成(false)
{
}

正oid UMin成Me設置o本yOpti設置ize本::Initialize()
{
    PoolStats.E設置pty();
    CachedOb大ects.E設置pty();
    A使toClean使pTi設置e本s.E設置pty();
    bIsMonito本in成 = false;

    // Initialize pool stats
    fo本 (int32 i = 0; i < (int32)EMin成Me設置o本yPoolType::Gene本al + 1; ++i)
    {
        EMin成Me設置o本yPoolType PoolType = (EMin成Me設置o本yPoolType)i;
        軍Min成Me設置o本yPoolStats Stats;
        Stats.PoolType = PoolType;
        PoolStats.Add(PoolType, Stats);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Me設置o本yOpti設置ize本 initialized"));
}

正oid UMin成Me設置o本yOpti設置ize本::Sh使tdown()
{
    StopMe設置o本yMonito本in成();

    // Cancel all a使to-clean使p ti設置e本s
    fo本 (const a使to& Pai本 : A使toClean使pTi設置e本s)
    {
        if (Get基本o本ld())
        {
            Get基本o本ld()->GetTi設置e本Mana成e本().Clea本Ti設置e本(Pai本.Val使e);
        }
    }
    A使toClean使pTi設置e本s.E設置pty();

    Clea本Cache();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Me設置o本yOpti設置ize本 sh使tdown"));
}

軍Min成Me設置o本yOpti設置izationRes使lt UMin成Me設置o本yOpti設置ize本::Opti設置izeMe設置o本y(EMin成Me設置o本ySt本ate成y St本ate成y)
{
    軍Min成Me設置o本yOpti設置izationRes使lt Res使lt;
    Res使lt.St本ate成y = St本ate成y;

    float Sta本tTi設置e = Get基本o本ld() 基本 Get基本o本ld()->GetTi設置eSeconds() : 0.0f;
    float Me設置o本yBefo本e = GetTotalMe設置o本yUsed();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本tin成 設置e設置o本y opti設置ization with st本ate成y: %s"), 
        *UEn使設置::GetVal使eAsSt本in成(St本ate成y));

    switch (St本ate成y)
    {
    case EMin成Me設置o本ySt本ate成y::Conse本正ati正e:
        // Gentle clean使p - 大使st t本i成成e本 GC
        Pe本fo本設置Ga本ba成eCollection(false);
        b本eak;

    case EMin成Me設置o本ySt本ate成y::Balanced:
        // Mode本ate clean使p
        Pe本fo本設置Ga本ba成eCollection(false);
        軍l使shText使本eSt本ea設置in成();
        UnloadUn使sedAssets(120.0f);
        b本eak;

    case EMin成Me設置o本ySt本ate成y::A成成本essi正e:
        // 輸入ea正y clean使p
        Pe本fo本設置Ga本ba成eCollection(t本使e);
        軍l使shAllPools();
        UnloadUn使sedAssets(30.0f);
        Clea本Cache();
        Co設置pactMe設置o本y();
        b本eak;

    case EMin成Me設置o本ySt本ate成y::E設置e本成ency:
        // E設置e本成ency clean使p
        Pe本fo本設置Ga本ba成eCollection(t本使e);
        軍l使shAllPools();
        Clea本Cache();
        Co設置pactMe設置o本y();
        // 軍o本ce clean使p of 設置o本e ob大ects
        fo本 (TActo本Ite本ato本<AActo本> It(Get基本o本ld()); It; ++It)
        {
            AActo本* Acto本 = *It;
            if (Acto本 && !Acto本->IsPendin成Kill() && !Acto本->IsA<APlaye本Cont本olle本>())
            {
                // Check if acto本 is fa本 f本o設置 playe本s
                Res使lt.的使設置Ob大ects軍本eed++;
            }
        }
        b本eak;
    }

    float Me設置o本yAfte本 = GetTotalMe設置o本yUsed();
    Res使lt.Me設置o本y軍本eedMB = Me設置o本yBefo本e - Me設置o本yAfte本;
    Res使lt.P本ocessin成Ti設置eMs = (Get基本o本ld() 基本 Get基本o本ld()->GetTi設置eSeconds() : 0.0f) - Sta本tTi設置e * 1000.0f;
    Res使lt.bS使ccessf使l = Res使lt.Me設置o本y軍本eedMB > 0;
    Res使lt.Details = 軍St本in成::P本intf(TEXT("軍本eed %.2f MB in %.2f 設置s"), Res使lt.Me設置o本y軍本eedMB, Res使lt.P本ocessin成Ti設置eMs);

    OnOpti設置izationCo設置pleted.B本oadcast(Res使lt);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Me設置o本y opti設置ization co設置pleted: %s"), *Res使lt.Details);

    本et使本n Res使lt;
}

軍Min成Me設置o本yOpti設置izationRes使lt UMin成Me設置o本yOpti設置ize本::Q使ickOpti設置ize()
{
    本et使本n Opti設置izeMe設置o本y(EMin成Me設置o本ySt本ate成y::Balanced);
}

軍Min成Me設置o本yOpti設置izationRes使lt UMin成Me設置o本yOpti設置ize本::DeepOpti設置ize()
{
    本et使本n Opti設置izeMe設置o本y(EMin成Me設置o本ySt本ate成y::A成成本essi正e);
}

軍Min成Me設置o本yOpti設置izationRes使lt UMin成Me設置o本yOpti設置ize本::E設置e本成encyClean使p()
{
    本et使本n Opti設置izeMe設置o本y(EMin成Me設置o本ySt本ate成y::E設置e本成ency);
}

正oid UMin成Me設置o本yOpti設置ize本::軍l使shPool(EMin成Me設置o本yPoolType PoolType)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍l使shin成 設置e設置o本y pool: %s"), *UEn使設置::GetVal使eAsSt本in成(PoolType));

    switch (PoolType)
    {
    case EMin成Me設置o本yPoolType::Text使本e:
        軍l使shText使本eSt本ea設置in成();
        b本eak;

    case EMin成Me設置o本yPoolType::Mesh:
        軍l使shStaticMeshCache();
        軍l使shSkeletalMeshCache();
        b本eak;

    case EMin成Me設置o本yPoolType::Ani設置ation:
        軍l使shAni設置ationCache();
        b本eak;

    case EMin成Me設置o本yPoolType::A使dio:
        軍l使shA使dioCache();
        b本eak;

    case EMin成Me設置o本yPoolType::AI:
        軍l使shAICache();
        b本eak;

    case EMin成Me設置o本yPoolType::的etwo本k:
        軍l使sh的etwo本kCache();
        b本eak;

    case EMin成Me設置o本yPoolType::UI:
        軍l使shUICache();
        b本eak;

    defa使lt:
        b本eak;
    }

    if (PoolStats.Contains(PoolType))
    {
        軍Min成Me設置o本yPoolStats& Stats = PoolStats[PoolType];
        Stats.UsedMe設置o本yMB = 0.0f;
        Stats.的使設置Allocations = 0;
    }
}

正oid UMin成Me設置o本yOpti設置ize本::軍l使shAllPools()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍l使shin成 all 設置e設置o本y pools"));

    fo本 (int32 i = 0; i < (int32)EMin成Me設置o本yPoolType::Gene本al + 1; ++i)
    {
        軍l使shPool((EMin成Me設置o本yPoolType)i);
    }
}

軍Min成Me設置o本yPoolStats UMin成Me設置o本yOpti設置ize本::GetPoolStats(EMin成Me設置o本yPoolType PoolType) const
{
    if (PoolStats.Contains(PoolType))
    {
        本et使本n PoolStats[PoolType];
    }

    本et使本n 軍Min成Me設置o本yPoolStats();
}

TA本本ay<軍Min成Me設置o本yPoolStats> UMin成Me設置o本yOpti設置ize本::GetAllPoolStats() const
{
    TA本本ay<軍Min成Me設置o本yPoolStats> Res使lt;
    fo本 (const a使to& Pai本 : PoolStats)
    {
        Res使lt.Add(Pai本.Val使e);
    }
    本et使本n Res使lt;
}

正oid UMin成Me設置o本yOpti設置ize本::Clea本Cache()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Clea本in成 ob大ect cache - %d ite設置s"), CachedOb大ects.的使設置());

    CachedOb大ects.E設置pty();
}

正oid UMin成Me設置o本yOpti設置ize本::SetCacheSizeLi設置it(float MaxSizeMB)
{
    CacheSizeLi設置itMB = 軍Math::Max(64.0f, MaxSizeMB);

    // If c使本本ent cache exceeds new li設置it, e正ict ite設置s
    if (GetCacheSize() > CacheSizeLi設置itMB)
    {
        E正ictLeastUsedCacheIte設置s(CacheSizeLi設置itMB * 0.8f);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Cache size li設置it set to %.2f MB"), CacheSizeLi設置itMB);
}

float UMin成Me設置o本yOpti設置ize本::GetCacheSize() const
{
    本et使本n Calc使lateCacheSize();
}

正oid UMin成Me設置o本yOpti設置ize本::UnloadUn使sedAssets(float Un使sedTi設置eTh本eshold)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Unloadin成 使n使sed assets (th本eshold: %.2f seconds)"), Un使sedTi設置eTh本eshold);

    float C使本本entTi設置e = Get基本o本ld() 基本 Get基本o本ld()->GetTi設置eSeconds() : 0.0f;
    int32 UnloadedCo使nt = 0;

    // Re設置o正e cached ob大ects that ha正en't been accessed 本ecently
    fo本 (int32 i = CachedOb大ects.的使設置() - 1; i >= 0; --i)
    {
        float Ti設置eSinceAccess = C使本本entTi設置e - CachedOb大ects[i].LastAccessTi設置e;
        if (Ti設置eSinceAccess > Un使sedTi設置eTh本eshold && CachedOb大ects[i].bCanUnload)
        {
            CachedOb大ects.Re設置o正eAt(i);
            UnloadedCo使nt++;
        }
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Unloaded %d 使n使sed cache ite設置s"), UnloadedCo使nt);
}

正oid UMin成Me設置o本yOpti設置ize本::Co設置pactMe設置o本y()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Co設置pactin成 設置e設置o本y"));

    // Co設置pact allocato本
    Co設置pactAllocato本();

    // 軍l使sh 本ende本 本eso使本ce cache to help def本a成設置entation
    軍l使shRende本Reso使本ceCache();
}

正oid UMin成Me設置o本yOpti設置ize本::Re成iste本Ob大ect軍o本A使toClean使p(UOb大ect* Ob大ect, float Lifeti設置eSeconds)
{
    if (!Ob大ect  !Get基本o本ld())
    {
        本et使本n;
    }

    // Cancel any existin成 clean使p ti設置e本 fo本 this ob大ect
    if (A使toClean使pTi設置e本s.Contains(Ob大ect))
    {
        Get基本o本ld()->GetTi設置e本Mana成e本().Clea本Ti設置e本(A使toClean使pTi設置e本s[Ob大ect]);
    }

    // Set 使p new clean使p ti設置e本
    軍Ti設置e本輸入andle Ti設置e本輸入andle;
    軍Ti設置e本Dele成ate Ti設置e本Dele成ate;
    Ti設置e本Dele成ate.BindLa設置bda([this, Ob大ect]()
    {
        if (Ob大ect && !Ob大ect->IsPendin成Kill())
        {
            Ob大ect->Ma本kPendin成Kill();
        }
        A使toClean使pTi設置e本s.Re設置o正e(Ob大ect);
    });

    Get基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(Ti設置e本輸入andle, Ti設置e本Dele成ate, Lifeti設置eSeconds, false);
    A使toClean使pTi設置e本s.Add(Ob大ect, Ti設置e本輸入andle);

    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Re成iste本ed ob大ect fo本 a使to-clean使p in %.2f seconds"), Lifeti設置eSeconds);
}

正oid UMin成Me設置o本yOpti設置ize本::CancelA使toClean使p(UOb大ect* Ob大ect)
{
    if (!Ob大ect  !A使toClean使pTi設置e本s.Contains(Ob大ect)  !Get基本o本ld())
    {
        本et使本n;
    }

    Get基本o本ld()->GetTi設置e本Mana成e本().Clea本Ti設置e本(A使toClean使pTi設置e本s[Ob大ect]);
    A使toClean使pTi設置e本s.Re設置o正e(Ob大ect);
}

正oid UMin成Me設置o本yOpti設置ize本::軍o本ceDest本oyOb大ect(UOb大ect* Ob大ect)
{
    if (!Ob大ect)
    {
        本et使本n;
    }

    CancelA使toClean使p(Ob大ect);

    if (!Ob大ect->IsPendin成Kill())
    {
        Ob大ect->Ma本kPendin成Kill();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("軍o本ce dest本oyed ob大ect: %s"), *Ob大ect->Get的a設置e());
    }
}

正oid UMin成Me設置o本yOpti設置ize本::SetText使本eSt本ea設置in成PoolSize(float SizeMB)
{
    // In 本eal i設置ple設置entation, this wo使ld set the text使本e st本ea設置in成 pool size
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Text使本e st本ea設置in成 pool size set to %.2f MB"), SizeMB);
}

正oid UMin成Me設置o本yOpti設置ize本::軍l使shText使本eSt本ea設置in成()
{
    // In 本eal i設置ple設置entation, this wo使ld fl使sh the text使本e st本ea設置in成 cache
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Text使本e st本ea設置in成 cache fl使shed"));

    if (PoolStats.Contains(EMin成Me設置o本yPoolType::Text使本e))
    {
        PoolStats[EMin成Me設置o本yPoolType::Text使本e].UsedMe設置o本yMB = 0.0f;
    }
}

正oid UMin成Me設置o本yOpti設置ize本::Red使ceText使本eResol使tions(int32 MaxText使本eSize)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Red使cin成 text使本e 本esol使tions to 設置ax %d"), MaxText使本eSize);

    // In 本eal i設置ple設置entation, this wo使ld ite本ate th本o使成h text使本es and 本ed使ce 本esol使tion
}

正oid UMin成Me設置o本yOpti設置ize本::軍l使shStaticMeshCache()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Static 設置esh cache fl使shed"));

    if (PoolStats.Contains(EMin成Me設置o本yPoolType::Mesh))
    {
        PoolStats[EMin成Me設置o本yPoolType::Mesh].UsedMe設置o本yMB = 0.0f;
    }
}

正oid UMin成Me設置o本yOpti設置ize本::軍l使shSkeletalMeshCache()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Skeletal 設置esh cache fl使shed"));
}

正oid UMin成Me設置o本yOpti設置ize本::Sta本tMe設置o本yMonito本in成(float Inte本正alSeconds)
{
    if (bIsMonito本in成)
    {
        本et使本n;
    }

    bIsMonito本in成 = t本使e;

    if (Get基本o本ld())
    {
        Get基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(Monito本in成Ti設置e本, this,
            &UMin成Me設置o本yOpti設置ize本::UpdateMe設置o本yStats, Inte本正alSeconds, t本使e);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Me設置o本y 設置onito本in成 sta本ted (inte本正al: %.2f seconds)"), Inte本正alSeconds);
}

正oid UMin成Me設置o本yOpti設置ize本::StopMe設置o本yMonito本in成()
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

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Me設置o本y 設置onito本in成 stopped"));
}

bool UMin成Me設置o本yOpti設置ize本::IsMe設置o本yMonito本in成() const
{
    本et使本n bIsMonito本in成;
}

float UMin成Me設置o本yOpti設置ize本::GetTotalMe設置o本yUsed() const
{
    軍Platfo本設置Me設置o本yStats Stats = 軍Platfo本設置Me設置o本y::GetStats();
    本et使本n (Stats.UsedVi本t使al - Stats.A正ailableVi本t使al) / (1024.0f * 1024.0f);
}

float UMin成Me設置o本yOpti設置ize本::GetTotalMe設置o本yA正ailable() const
{
    軍Platfo本設置Me設置o本yStats Stats = 軍Platfo本設置Me設置o本y::GetStats();
    本et使本n Stats.A正ailableVi本t使al / (1024.0f * 1024.0f);
}

float UMin成Me設置o本yOpti設置ize本::GetMe設置o本yUsa成ePe本cent() const
{
    軍Platfo本設置Me設置o本yStats Stats = 軍Platfo本設置Me設置o本y::GetStats();
    float TotalPhysical = Stats.TotalPhysical / (1024.0f * 1024.0f);
    float UsedPhysical = (Stats.UsedPhysical) / (1024.0f * 1024.0f);

    if (TotalPhysical <= 0.0f)
    {
        本et使本n 0.0f;
    }

    本et使本n (UsedPhysical / TotalPhysical) * 100.0f;
}

正oid UMin成Me設置o本yOpti設置ize本::SetMe設置o本y基本a本nin成Th本eshold(float Pe本cent)
{
    Me設置o本y基本a本nin成Th本eshold = 軍Math::Cla設置p(Pe本cent, 50.0f, 95.0f);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Me設置o本y wa本nin成 th本eshold set to %.1f%%"), Me設置o本y基本a本nin成Th本eshold);
}

float UMin成Me設置o本yOpti設置ize本::GetMe設置o本y基本a本nin成Th本eshold() const
{
    本et使本n Me設置o本y基本a本nin成Th本eshold;
}

軍St本in成 UMin成Me設置o本yOpti設置ize本::GetMe設置o本yRepo本t() const
{
    軍St本in成 Repo本t = TEXT("=== Min成GoRTS Me設置o本y Repo本t ===\n");
    Repo本t += 軍St本in成::P本intf(TEXT("Total Me設置o本y Used: %.2f MB\n"), GetTotalMe設置o本yUsed());
    Repo本t += 軍St本in成::P本intf(TEXT("Total Me設置o本y A正ailable: %.2f MB\n"), GetTotalMe設置o本yA正ailable());
    Repo本t += 軍St本in成::P本intf(TEXT("Me設置o本y Usa成e: %.1f%%\n"), GetMe設置o本yUsa成ePe本cent());
    Repo本t += 軍St本in成::P本intf(TEXT("Cache Size: %.2f MB / %.2f MB\n"), GetCacheSize(), CacheSizeLi設置itMB);
    Repo本t += 軍St本in成::P本intf(TEXT("Cache Ite設置s: %d\n"), CachedOb大ects.的使設置());
    Repo本t += 軍St本in成::P本intf(TEXT("A使to-Clean使p Ob大ects: %d\n"), A使toClean使pTi設置e本s.的使設置());
    Repo本t += 軍St本in成::P本intf(TEXT("Me設置o本y 基本a本nin成 Th本eshold: %.1f%%\n"), Me設置o本y基本a本nin成Th本eshold);

    Repo本t += TEXT("\n=== Pool Statistics ===\n");
    fo本 (const a使to& Pai本 : PoolStats)
    {
        const 軍Min成Me設置o本yPoolStats& Stats = Pai本.Val使e;
        Repo本t += 軍St本in成::P本intf(TEXT("%s: %.2f MB, %d allocations\n"),
            *UEn使設置::GetVal使eAsSt本in成(Stats.PoolType), Stats.UsedMe設置o本yMB, Stats.的使設置Allocations);
    }

    本et使本n Repo本t;
}

正oid UMin成Me設置o本yOpti設置ize本::Lo成Me設置o本yStats()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("%s"), *GetMe設置o本yRepo本t());
}

// Inte本nal helpe本 f使nctions

正oid UMin成Me設置o本yOpti設置ize本::UpdateMe設置o本yStats()
{
    // Update pool stats
    fo本 (a使to& Pai本 : PoolStats)
    {
        軍Min成Me設置o本yPoolStats& Stats = Pai本.Val使e;
        // In 本eal i設置ple設置entation, this wo使ld 成athe本 act使al stats
        Stats.UsedMe設置o本yMB = 軍Math::RandRan成e(10.0f, 100.0f);
        Stats.的使設置Allocations = 軍Math::RandRan成e(100, 1000);
    }

    // Check 設置e設置o本y wa本nin成s
    CheckMe設置o本y基本a本nin成s();
}

正oid UMin成Me設置o本yOpti設置ize本::CheckMe設置o本y基本a本nin成s()
{
    float Usa成ePe本cent = GetMe設置o本yUsa成ePe本cent();

    if (Usa成ePe本cent > Me設置o本y基本a本nin成Th本eshold)
    {
        OnMe設置o本y基本a本nin成.B本oadcast(Usa成ePe本cent, Me設置o本y基本a本nin成Th本eshold);

        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Me設置o本y 使sa成e wa本nin成: %.1f%% (th本eshold: %.1f%%)"),
            Usa成ePe本cent, Me設置o本y基本a本nin成Th本eshold);
    }
}

float UMin成Me設置o本yOpti設置ize本::Calc使lateCacheSize() const
{
    float TotalSize = 0.0f;
    fo本 (const a使to& Info : CachedOb大ects)
    {
        TotalSize += Info.Me設置o本ySize;
    }
    本et使本n TotalSize / (1024.0f * 1024.0f); // Con正e本t to MB
}

正oid UMin成Me設置o本yOpti設置ize本::E正ictLeastUsedCacheIte設置s(float Ta本成etSizeMB)
{
    // So本t by access ti設置e (oldest fi本st)
    CachedOb大ects.So本t([](const 軍Min成CachedOb大ectInfo& A, const 軍Min成CachedOb大ectInfo& B)
    {
        本et使本n A.LastAccessTi設置e < B.LastAccessTi設置e;
    });

    // Re設置o正e ite設置s 使ntil we'本e 使nde本 ta本成et
    while (Calc使lateCacheSize() > Ta本成etSizeMB && CachedOb大ects.的使設置() > 0)
    {
        CachedOb大ects.Re設置o正eAt(0);
    }
}

正oid UMin成Me設置o本yOpti設置ize本::軍l使shRende本Reso使本ceCache()
{
    // In 本eal i設置ple設置entation, this wo使ld fl使sh 本ende本 本eso使本ces
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Rende本 本eso使本ce cache fl使shed"));
}

正oid UMin成Me設置o本yOpti設置ize本::軍l使shAni設置ationCache()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ani設置ation cache fl使shed"));
}

正oid UMin成Me設置o本yOpti設置ize本::軍l使shA使dioCache()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("A使dio cache fl使shed"));
}

正oid UMin成Me設置o本yOpti設置ize本::軍l使shAICache()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("AI cache fl使shed"));
}

正oid UMin成Me設置o本yOpti設置ize本::軍l使sh的etwo本kCache()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("的etwo本k cache fl使shed"));
}

正oid UMin成Me設置o本yOpti設置ize本::軍l使shUICache()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("UI cache fl使shed"));
}

正oid UMin成Me設置o本yOpti設置ize本::Pe本fo本設置Ga本ba成eCollection(bool b軍使llP使本成e)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置in成 成a本ba成e collection (f使ll p使本成e: %s)"),
        b軍使llP使本成e 基本 TEXT("Yes") : TEXT("的o"));

    CollectGa本ba成e(GARBAGE下COLLECTIO的下KEEP軍LAGS);
}

正oid UMin成Me設置o本yOpti設置ize本::Co設置pactAllocato本()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Co設置pactin成 設置e設置o本y allocato本"));
}
