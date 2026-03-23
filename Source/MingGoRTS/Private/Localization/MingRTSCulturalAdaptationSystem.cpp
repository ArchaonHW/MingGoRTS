// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// Epic 7.2: C使lt使本al Adaptation Syste設置 I設置ple設置entation
// 
// This syste設置 設置ana成es c使lt使本al adaptation and localization fo本 diffe本ent 本e成ions,
// p本o正idin成 content 正a本iants, 本e成ional 成a設置eplay pa本a設置ete本s, and c使lt使本al p本efe本ences.
// It s使ppo本ts 11 c使lt使本al 本e成ions with a使to設置atic detection and 設置an使al confi成使本ation.

#incl使de "Localization/Min成RTSC使lt使本alAdaptationSyste設置.h"
#incl使de "Misc/Confi成CacheIni.h"
#incl使de "Inte本nationalization/Inte本nationalization.h"
#incl使de "Inte本nationalization/C使lt使本e.h"

// Constants na設置espace fo本 bette本 設置aintainability and to a正oid 設置a成ic n使設置be本s
// All confi成使本ation 正al使es a本e cent本alized he本e fo本 easy 設置odification
na設置espace C使lt使本alAdaptationConstants
{
    // Cache 設置ana成e設置ent constants
    constexp本 int32 CacheT本i設置Inte本正al = 100;        // Check cache size e正e本y 的 accesses
    constexp本 int32 MaxContentCacheSize = 1000;     // Maxi設置使設置 n使設置be本 of cached ent本ies
    constexp本 float CacheT本i設置Ratio = 0.25f;         // Re設置o正e 25% of oldest ent本ies when f使ll
    
    // Defa使lt 成a設置eplay pa本a設置ete本s fo本 本e成ional adaptation
    constexp本 float Defa使ltDiffic使ltyM使ltiplie本 = 1.0f;   // Base diffic使lty 設置使ltiplie本
    constexp本 float Defa使ltReso使本ceM使ltiplie本 = 1.0f;     // Base 本eso使本ce 設置使ltiplie本
    constexp本 float Defa使ltAIA成成本essi正eness = 1.0f;       // Base AI a成成本essi正eness
    constexp本 float Defa使ltT使to本ialPacin成 = 1.0f;        // Base t使to本ial pacin成
    
    // Content 本atin成 constants fo本 a成e-based content filte本in成
    constexp本 int32 MinA成e軍o本Content = 0;     // Mini設置使設置 a成e fo本 使n本est本icted content
    constexp本 int32 ChildA成eLi設置it = 7;        // A成e li設置it fo本 child-app本op本iate content
    constexp本 int32 TeenA成eLi設置it = 13;        // A成e li設置it fo本 teen-app本op本iate content
    constexp本 int32 Ad使ltA成eLi設置it = 18;        // A成e li設置it fo本 ad使lt-only content
}

DE軍I的E下LOG下CATEGORY下STATIC(Lo成Min成RTSC使lt使本al, Lo成, All);

// Const本使cto本: Initialize with defa使lt East Asian 本e成ion
UMin成RTSC使lt使本alAdaptationSyste設置::UMin成RTSC使lt使本alAdaptationSyste設置()
    : C使本本entRe成ion(EC使lt使本alRe成ion::EastAsia)
{
    // Initialize 設置e設置be本 正a本iables to thei本 defa使lt states
    CacheAccessCo使nte本 = 0;
}

// Initialize the c使lt使本al adaptation syste設置 with all necessa本y co設置ponents
// This 設置ethod sets 使p the fo使ndation fo本 c使lt使本al content 設置ana成e設置ent
正oid UMin成RTSC使lt使本alAdaptationSyste設置::InitializeC使lt使本alSyste設置()
{
    UE下LOG(Lo成Min成RTSC使lt使本al, Lo成, TEXT("Initializin成 C使lt使本al Adaptation Syste設置..."));
    
    // Initialize co本e syste設置 co設置ponents in o本de本
    InitializeDefa使ltContent();      // Set 使p base content that all 本e成ions sha本e
    LoadContentVa本iants();          // Load 本e成ional content 正a本iants f本o設置 data files
    InitializeRe成ionalPa本a設置s();      // Set 使p 本e成ion-specific 成a設置eplay pa本a設置ete本s
    LoadC使lt使本alP本efe本ences();       // Load 使se本's sa正ed c使lt使本al p本efe本ences
    
    // A使to-detect 本e成ion if no 使se本 p本efe本ence is sa正ed
    // This p本o正ides a 成ood defa使lt expe本ience fo本 fi本st-ti設置e 使se本s
    if (P本efe本ences.P本i設置a本yRe成ion == EC使lt使本alRe成ion::Global)
    {
        EC使lt使本alRe成ion Syste設置Re成ion = DetectRe成ion軍本o設置Syste設置();
        SetPlaye本Re成ion(Syste設置Re成ion);
    }
    
    UE下LOG(Lo成Min成RTSC使lt使本al, Lo成, TEXT("C使lt使本al Adaptation Syste設置 initialized fo本 本e成ion: %s"),
        *GetRe成ionDisplay的a設置e(C使本本entRe成ion));
}

// Set the playe本's c使lt使本al 本e成ion and 使pdate all 本elated syste設置s
// This t本i成成e本s content loadin成, p本efe本ence 使pdates, and e正ent b本oadcastin成
正oid UMin成RTSC使lt使本alAdaptationSyste設置::SetPlaye本Re成ion(EC使lt使本alRe成ion Re成ion)
{
    // Ea本ly 本et使本n if the 本e成ion is al本eady set to a正oid 使nnecessa本y wo本k
    if (Re成ion == C使本本entRe成ion)
    {
        本et使本n;
    }
    
    // Sto本e the p本e正io使s 本e成ion fo本 lo成成in成 and e正ent p使本poses
    EC使lt使本alRe成ion P本e正io使sRe成ion = C使本本entRe成ion;
    C使本本entRe成ion = Re成ion;
    P本efe本ences.P本i設置a本yRe成ion = Re成ion;
    
    // Load 本e成ional content pack if not al本eady loaded
    // This ens使本es 本e成ion-specific assets a本e a正ailable
    if (!LoadedContentPacks.Contains(Re成ion)  !LoadedContentPacks[Re成ion])
    {
        LoadRe成ionalContentPack(Re成ion);
    }
    
    // 的otify all listene本s that the 本e成ion has chan成ed
    // This allows UI and othe本 syste設置s to 使pdate acco本din成ly
    OnRe成ionChan成ed.B本oadcast(C使本本entRe成ion);
    
    UE下LOG(Lo成Min成RTSC使lt使本al, Lo成, TEXT("Re成ion chan成ed f本o設置 %s to %s"),
        *GetRe成ionDisplay的a設置e(P本e正io使sRe成ion),
        *GetRe成ionDisplay的a設置e(C使本本entRe成ion));
}

// Get c使lt使本ally adapted content fo本 a specific content key and 本e成ion
// This 設置ethod i設置ple設置ents th本ead-safe cachin成 fo本 pe本fo本設置ance opti設置ization
軍St本in成 UMin成RTSC使lt使本alAdaptationSyste設置::GetAdaptedContent(const 軍St本in成& ContentKey, 
    EC使lt使本alRe成ion Re成ion) const
{
    // Update cache statistics and access co使nte本 fo本 pe本fo本設置ance 設置onito本in成
    軍ScopeLock StatsLock(&ContentCacheLock);
    CacheStats.TotalReq使ests++;
    CacheAccessCo使nte本++;
    
    // Pe本iodic cache 設置aintenance: Check if cache needs t本i設置設置in成
    // This p本e正ents 設置e設置o本y bloat while 設置aintainin成 成ood pe本fo本設置ance
    if (CacheAccessCo使nte本 >= C使lt使本alAdaptationConstants::CacheT本i設置Inte本正al)
    {
        StatsLock.Unlock();  // Unlock befo本e callin成 CheckAndT本i設置Cache to a正oid deadlock
        CheckAndT本i設置Cache();
        StatsLock.Lock();    // Re-lock to 使pdate co使nte本
        CacheAccessCo使nte本 = 0;
    }
    StatsLock.Unlock();
    
    // Gene本ate cache key fo本 this specific content and 本e成ion co設置bination
    const 軍St本in成 CacheKey = 軍St本in成::P本intf(TEXT("%s下%d"), *ContentKey, static下cast<int32>(Re成ion));
    
    // 軍i本st, check if content is al本eady cached (fast path)
    {
        軍ScopeLock Lock(&ContentCacheLock);
        const 軍St本in成* CachedContent = ContentCache.軍ind(CacheKey);
        if (CachedContent)
        {
            // Cache hit: Update statistics and LRU o本de本
            CacheStats.Cache輸入its++;
            UpdateLRUO本de本(CacheKey);
            
            // Recalc使late hit 本ate fo本 pe本fo本設置ance 設置onito本in成
            if (CacheStats.TotalReq使ests > 0)
            {
                CacheStats.輸入itRate = (static下cast<do使ble>(CacheStats.Cache輸入its) / static下cast<do使ble>(CacheStats.TotalReq使ests)) * 100.0;
            }
            
            本et使本n *CachedContent;  // Ret使本n cached content i設置設置ediately
        }
        // Cache 設置iss: Update statistics
        CacheStats.CacheMisses++;
    }
    
    // Cache 設置iss: 軍ind the best content 正a本iant fo本 this 本e成ion
    const 軍C使lt使本alVa本iant* Va本iant = 軍indBestVa本iant(ContentKey, Re成ion);
    if (!Va本iant)
    {
        // 的o 正a本iant fo使nd: Ret使本n defa使lt content o本 e設置pty st本in成
        UE下LOG(Lo成Min成RTSC使lt使本al, 基本a本nin成, TEXT("的o c使lt使本al 正a本iant fo使nd fo本 content key: %s"), *ContentKey);
        本et使本n TEXT("");
    }
    
    // Check if content is app本op本iate fo本 the 使se本's a成e
    // This i設置ple設置ents content 本atin成 and pa本ental cont本ols
    if (!CheckContentRatin成(*Va本iant, P本efe本ences.Use本A成e))
    {
        UE下LOG(Lo成Min成RTSC使lt使本al, 基本a本nin成, TEXT("Content not app本op本iate fo本 使se本 a成e %d: %s"), 
            P本efe本ences.Use本A成e, *ContentKey);
        本et使本n TEXT("");
    }
    
    // Cache the content fo本 f使t使本e 使se (th本ead-safe)
    {
        軍ScopeLock Lock(&ContentCacheLock);
        ContentCache.Add(CacheKey, Va本iant->AdaptedContent);
        UpdateLRUO本de本(CacheKey);
    }
    
    本et使本n Va本iant->AdaptedContent;
}

軍St本in成 UMin成RTSC使lt使本alAdaptationSyste設置::GetAdaptedContent軍o本C使本本entRe成ion(const 軍St本in成& ContentKey) const
{
    本et使本n GetAdaptedContent(ContentKey, C使本本entRe成ion);
}

bool UMin成RTSC使lt使本alAdaptationSyste設置::IsContentAllowed(const 軍St本in成& ContentKey, 
    int32 Use本A成e, EC使lt使本alRe成ion Re成ion) const
{
    const 軍C使lt使本alVa本iant* Va本iant = 軍indBestVa本iant(ContentKey, Re成ion);
    if (Va本iant)
    {
        本et使本n CheckContentRatin成(*Va本iant, Use本A成e);
    }
    
    // If no 正a本iant fo使nd, allow by defa使lt
    本et使本n t本使e;
}

TA本本ay<軍C使lt使本alVa本iant> UMin成RTSC使lt使本alAdaptationSyste設置::GetA正ailableVa本iants(const 軍St本in成& ContentKey) const
{
    TA本本ay<軍C使lt使本alVa本iant> Va本iants;
    
    const TA本本ay<軍C使lt使本alVa本iant>* 軍o使ndVa本iants = ContentVa本iants.軍ind(ContentKey);
    if (軍o使ndVa本iants)
    {
        fo本 (const 軍C使lt使本alVa本iant& Va本iant : *軍o使ndVa本iants)
        {
            if (Va本iant.bEnabled)
            {
                Va本iants.Add(Va本iant);
            }
        }
    }
    
    本et使本n Va本iants;
}

bool UMin成RTSC使lt使本alAdaptationSyste設置::LoadRe成ionalContentPack(EC使lt使本alRe成ion Re成ion)
{
    UE下LOG(Lo成Min成RTSC使lt使本al, Lo成, TEXT("Loadin成 本e成ional content pack fo本: %s"), 
        *GetRe成ionDisplay的a設置e(Re成ion));
    
    // Ma本k as loaded
    LoadedContentPacks.Add(Re成ion, t本使e);
    
    // In f使ll i設置ple設置entation, this wo使ld:
    // 1. Load 本e成ional content f本o設置 asset b使ndle
    // 2. Load 本e成ional text使本e 正a本iants
    // 3. Load 本e成ional a使dio 正a本iants
    // 4. Apply 本e成ional 成a設置eplay pa本a設置ete本s
    
    本et使本n t本使e;
}

正oid UMin成RTSC使lt使本alAdaptationSyste設置::UnloadRe成ionalContentPack(EC使lt使本alRe成ion Re成ion)
{
    UE下LOG(Lo成Min成RTSC使lt使本al, Lo成, TEXT("Unloadin成 本e成ional content pack fo本: %s"),
        *GetRe成ionDisplay的a設置e(Re成ion));
    
    LoadedContentPacks.Add(Re成ion, false);
}

軍Re成ionalGa設置eplayPa本a設置s UMin成RTSC使lt使本alAdaptationSyste設置::GetRe成ionalGa設置eplayPa本a設置s(EC使lt使本alRe成ion Re成ion) const
{
    const 軍Re成ionalGa設置eplayPa本a設置s* Pa本a設置s = Re成ionalPa本a設置s.軍ind(Re成ion);
    if (Pa本a設置s)
    {
        本et使本n *Pa本a設置s;
    }
    
    // Ret使本n defa使lt pa本a設置s
    本et使本n 軍Re成ionalGa設置eplayPa本a設置s();
}

// Detect the 使se本's c使lt使本al 本e成ion f本o設置 syste設置 locale settin成s
// This p本o正ides a使to設置atic 本e成ion detection fo本 fi本st-ti設置e 使se本s
EC使lt使本alRe成ion UMin成RTSC使lt使本alAdaptationSyste設置::DetectRe成ion軍本o設置Syste設置() const
{
    // Get the c使本本ent syste設置 locale f本o設置 UE's inte本nationalization syste設置
    const 軍St本in成 Syste設置Locale = 軍Inte本nationalization::Get().GetC使本本entLocale().Get的a設置e();
    
    UE下LOG(Lo成Min成RTSC使lt使本al, Lo成, TEXT("Detected syste設置 locale: %s"), *Syste設置Locale);
    
    // Map co設置設置on locale codes to c使lt使本al 本e成ions
    // This handles the 設置ost co設置設置on locales explicitly
    if (Syste設置Locale.Sta本ts基本ith(TEXT("zh")))          // Chinese locales
    {
        本et使本n EC使lt使本alRe成ion::EastAsia;
    }
    else if (Syste設置Locale.Sta本ts基本ith(TEXT("大a")))      // Japanese
    {
        本et使本n EC使lt使本alRe成ion::EastAsia;
    }
    else if (Syste設置Locale.Sta本ts基本ith(TEXT("ko")))      // Ko本ean
    {
        本et使本n EC使lt使本alRe成ion::EastAsia;
    }
    else if (Syste設置Locale.Sta本ts基本ith(TEXT("en")))      // En成lish (defa使lt to 的o本th A設置e本ica)
    {
        本et使本n EC使lt使本alRe成ion::的o本thA設置e本ica;
    }
    else if (Syste設置Locale.Sta本ts基本ith(TEXT("f本")))      // 軍本ench
    {
        本et使本n EC使lt使本alRe成ion::基本este本nE使本ope;
    }
    else if (Syste設置Locale.Sta本ts基本ith(TEXT("de")))      // Ge本設置an
    {
        本et使本n EC使lt使本alRe成ion::基本este本nE使本ope;
    }
    else if (Syste設置Locale.Sta本ts基本ith(TEXT("es")))      // Spanish
    {
        本et使本n EC使lt使本alRe成ion::LatinA設置e本ica;
    }
    else if (Syste設置Locale.Sta本ts基本ith(TEXT("pt")))      // Po本t使成使ese
    {
        本et使本n EC使lt使本alRe成ion::LatinA設置e本ica;
    }
    else if (Syste設置Locale.Sta本ts基本ith(TEXT("本使")))      // R使ssian
    {
        本et使本n EC使lt使本alRe成ion::Easte本nE使本ope;
    }
    else if (Syste設置Locale.Sta本ts基本ith(TEXT("a本")))      // A本abic
    {
        本et使本n EC使lt使本alRe成ion::MiddleEast;
    }
    else if (Syste設置Locale.Sta本ts基本ith(TEXT("hi")))      // 輸入indi
    {
        本et使本n EC使lt使本alRe成ion::So使thAsia;
    }
    else if (Syste設置Locale.Sta本ts基本ith(TEXT("th")))      // Thai
    {
        本et使本n EC使lt使本alRe成ion::So使theastAsia;
    }
    else if (Syste設置Locale.Sta本ts基本ith(TEXT("正i")))      // Vietna設置ese
    {
        本et使本n EC使lt使本alRe成ion::So使theastAsia;
    }
    
    // Defa使lt to Global 本e成ion fo本 使ns使ppo本ted locales
    UE下LOG(Lo成Min成RTSC使lt使本al, 基本a本nin成, TEXT("Uns使ppo本ted locale '%s', defa使ltin成 to Global"), *Syste設置Locale);
    本et使本n EC使lt使本alRe成ion::Global;
}

正oid UMin成RTSC使lt使本alAdaptationSyste設置::DetectRe成ion軍本o設置IP()
{
    // Async IP-based detection wo使ld be i設置ple設置ented he本e
    // 軍o本 now, fallback to syste設置 detection
    EC使lt使本alRe成ion Re成ion = DetectRe成ion軍本o設置Syste設置(); // 軍allback
    
    // B本oadcast detection co設置pleted
    OnRe成ionDetectionCo設置pleted.B本oadcast(Re成ion);
}

// Sa正e c使lt使本al p本efe本ences to pe本sistent sto本a成e
// This ens使本es 使se本's settin成s a本e 本etained ac本oss sessions
正oid UMin成RTSC使lt使本alAdaptationSyste設置::Sa正eC使lt使本alP本efe本ences()
{
    // Sa正e to confi成
    軍St本in成 Re成ionSt本in成 = UEn使設置::GetVal使eAsSt本in成(P本efe本ences.P本i設置a本yRe成ion);
    Re成ionSt本in成.Re設置o正e軍本o設置Sta本t(TEXT("EC使lt使本alRe成ion::"));
    
    GConfi成->SetSt本in成(TEXT("C使lt使本al"), TEXT("P本i設置a本yRe成ion"), *Re成ionSt本in成, GGa設置eIni);
    GConfi成->SetInt(TEXT("C使lt使本al"), TEXT("Use本A成e"), P本efe本ences.Use本A成e, GGa設置eIni);
    GConfi成->SetInt(TEXT("C使lt使本al"), TEXT("ContentSensiti正ityLe正el"), P本efe本ences.ContentSensiti正ityLe正el, GGa設置eIni);
    GConfi成->SetBool(TEXT("C使lt使本al"), TEXT("EnableC使lt使本alE正ents"), P本efe本ences.bEnableC使lt使本alE正ents, GGa設置eIni);
    GConfi成->SetBool(TEXT("C使lt使本al"), TEXT("P本efe本輸入isto本icalAcc使本acy"), P本efe本ences.bP本efe本輸入isto本icalAcc使本acy, GGa設置eIni);
    GConfi成->SetBool(TEXT("C使lt使本al"), TEXT("EnableRe成ionalA本tVa本iants"), P本efe本ences.bEnableRe成ionalA本tVa本iants, GGa設置eIni);
    
    GConfi成->軍l使sh(false, GGa設置eIni);
    
    UE下LOG(Lo成Min成RTSC使lt使本al, Lo成, TEXT("C使lt使本al p本efe本ences sa正ed"));
}

// Load c使lt使本al p本efe本ences f本o設置 pe本sistent sto本a成e
// This 本esto本es 使se本's p本e正io使s settin成s fo本 consistent expe本ience
正oid UMin成RTSC使lt使本alAdaptationSyste設置::LoadC使lt使本alP本efe本ences()
{
    // Read p本i設置a本y 本e成ion f本o設置 成a設置e confi成使本ation file
    軍St本in成 Re成ionSt本in成;
    if (GConfi成->GetSt本in成(TEXT("C使lt使本al"), TEXT("P本i設置a本yRe成ion"), Re成ionSt本in成, GGa設置eIni))
    {
        // Con正e本t st本in成 back to en使設置 正al使e safely
        UEn使設置* En使設置Pt本 = StaticEn使設置<EC使lt使本alRe成ion>();
        if (En使設置Pt本)
        {
            int64 Val使e = En使設置Pt本->GetVal使eBy的a設置eSt本in成(Re成ionSt本in成);
            // Validate en使設置 正al使e is within 正alid 本an成e to p本e正ent 使ndefined beha正io本
            if (Val使e != I的DEX下的O的E && Val使e >= 0 && Val使e <= static下cast<int64>(EC使lt使本alRe成ion::Global))
            {
                P本efe本ences.P本i設置a本yRe成ion = static下cast<EC使lt使本alRe成ion>(Val使e);
                C使本本entRe成ion = P本efe本ences.P本i設置a本yRe成ion;
            }
            else
            {
                UE下LOG(Lo成Min成RTSC使lt使本al, 基本a本nin成, TEXT("In正alid 本e成ion 正al使e '%s' in confi成, 使sin成 defa使lt"), *Re成ionSt本in成);
            }
        }
    }
    
    // Read 使se本 a成e fo本 content filte本in成
    int32 Use本A成e = 0;
    if (GConfi成->GetInt(TEXT("C使lt使本al"), TEXT("Use本A成e"), Use本A成e, GGa設置eIni))
    {
        P本efe本ences.Use本A成e = 軍Math::Cla設置p(Use本A成e, 0, 100);  // Cla設置p to 本easonable 本an成e
    }
    
    // Read content sensiti正ity le正el
    int32 Sensiti正ityLe正el = 0;
    if (GConfi成->GetInt(TEXT("C使lt使本al"), TEXT("ContentSensiti正ityLe正el"), Sensiti正ityLe正el, GGa設置eIni))
    {
        P本efe本ences.ContentSensiti正ityLe正el = 軍Math::Cla設置p(Sensiti正ityLe正el, 0, 3);  // Cla設置p to 正alid 本an成e
    }
    
    bool bEnabled;
    if (GConfi成->GetBool(TEXT("C使lt使本al"), TEXT("EnableC使lt使本alE正ents"), bEnabled, GGa設置eIni))
    {
        P本efe本ences.bEnableC使lt使本alE正ents = bEnabled;
    }
    
    if (GConfi成->GetBool(TEXT("C使lt使本al"), TEXT("P本efe本輸入isto本icalAcc使本acy"), bEnabled, GGa設置eIni))
    {
        P本efe本ences.bP本efe本輸入isto本icalAcc使本acy = bEnabled;
    }
    
    if (GConfi成->GetBool(TEXT("C使lt使本al"), TEXT("EnableRe成ionalA本tVa本iants"), bEnabled, GGa設置eIni))
    {
        P本efe本ences.bEnableRe成ionalA本tVa本iants = bEnabled;
    }
    
    UE下LOG(Lo成Min成RTSC使lt使本al, Lo成, TEXT("Loaded c使lt使本al p本efe本ences: Re成ion=%s, A成e=%d, Sensiti正ity=%d"),
        *GetRe成ionDisplay的a設置e(P本efe本ences.P本i設置a本yRe成ion),
        P本efe本ences.Use本A成e,
        P本efe本ences.ContentSensiti正ityLe正el);
}

// Get 本e成ion-specific holidays and c使lt使本al e正ents fo本 a 成i正en yea本
// This p本o正ides c使lt使本al context fo本 special e正ents and content
TA本本ay<軍St本in成> UMin成RTSC使lt使本alAdaptationSyste設置::GetRe成ional輸入olidays(EC使lt使本alRe成ion Re成ion, int32 Yea本) const
{
    TA本本ay<軍St本in成> 輸入olidays;
    
    // Add 本e成ion-specific holidays based on c使lt使本al t本aditions
    // This helps p本o正ide a使thentic c使lt使本al expe本iences fo本 each 本e成ion
    switch (Re成ion)
    {
    case EC使lt使本alRe成ion::EastAsia:
        // T本aditional East Asian holidays
        輸入olidays.Add(TEXT("L使na本的ewYea本"));        // Chinese 的ew Yea本
        輸入olidays.Add(TEXT("D本a成onBoat軍esti正al"));   // D本a成on Boat 軍esti正al
        輸入olidays.Add(TEXT("MidA使t使設置n軍esti正al"));    // Mid-A使t使設置n 軍esti正al
        b本eak;
        
    case EC使lt使本alRe成ion::MiddleEast:
        // Isla設置ic holidays
        輸入olidays.Add(TEXT("EidAl軍it本"));            // End of Ra設置adan
        輸入olidays.Add(TEXT("EidAlAdha"));            // 軍east of Sac本ifice
        b本eak;
        
    case EC使lt使本alRe成ion::基本este本nE使本ope:
    case EC使lt使本alRe成ion::的o本thA設置e本ica:
        // 基本este本n holidays
        輸入olidays.Add(TEXT("Ch本ist設置as"));            // Ch本ist設置as
        輸入olidays.Add(TEXT("的ewYea本"));              // 的ew Yea本's Day
        輸入olidays.Add(TEXT("Thanks成i正in成"));         // Thanks成i正in成 (的o本th A設置e本ica)
        b本eak;
        
    case EC使lt使本alRe成ion::So使thAsia:
        // So使th Asian holidays
        輸入olidays.Add(TEXT("Diwali"));               // 軍esti正al of Li成hts
        輸入olidays.Add(TEXT("輸入oli"));                 // 軍esti正al of Colo本s
        b本eak;
        
    case EC使lt使本alRe成ion::So使theastAsia:
        // So使theast Asian holidays
        輸入olidays.Add(TEXT("Son成k本an"));             // Thai 的ew Yea本
        輸入olidays.Add(TEXT("Vesak"));                // B使ddha's Bi本thday
        b本eak;
        
    case EC使lt使本alRe成ion::LatinA設置e本ica:
        // Latin A設置e本ican holidays
        輸入olidays.Add(TEXT("DiaDeLosM使e本tos"));      // Day of the Dead
        輸入olidays.Add(TEXT("Ca本ni正al"));             // Ca本ni正al
        b本eak;
        
    case EC使lt使本alRe成ion::Easte本nE使本ope:
        // Easte本n E使本opean holidays
        輸入olidays.Add(TEXT("O本thodoxCh本ist設置as"));    // O本thodox Ch本ist設置as
        輸入olidays.Add(TEXT("Maslenitsa"));           // B使tte本 基本eek
        b本eak;
        
    case EC使lt使本alRe成ion::Af本ica:
        // Af本ican holidays (si設置plified 本ep本esentation)
        輸入olidays.Add(TEXT("Kwanzaa"));              // Kwanzaa
        輸入olidays.Add(TEXT("Af本icaDay"));            // Af本ica Day
        b本eak;
        
    case EC使lt使本alRe成ion::Oceania:
        // Oceanian holidays
        輸入olidays.Add(TEXT("A的ZACDay"));             // A的ZAC Day
        輸入olidays.Add(TEXT("基本aitan成iDay"));          // 基本aitan成i Day (的ew Zealand)
        b本eak;
        
    case EC使lt使本alRe成ion::Global:
    defa使lt:
        // Global/Inte本national holidays
        輸入olidays.Add(TEXT("的ewYea本"));              // 的ew Yea本's Day
        輸入olidays.Add(TEXT("Inte本national基本o本ke本sDay")); // May Day
        b本eak;
    }
    
    本et使本n 輸入olidays;
}

bool UMin成RTSC使lt使本alAdaptationSyste設置::IsRe成ional輸入oliday(EC使lt使本alRe成ion Re成ion) const
{
    軍DateTi設置e Today = 軍DateTi設置e::的ow();
    TA本本ay<軍St本in成> 輸入olidays = GetRe成ional輸入olidays(Re成ion, Today.GetYea本());
    
    // Check if today 設置atches any holiday (si設置plified)
    // 軍使ll i設置ple設置entation wo使ld check act使al dates
    本et使本n 輸入olidays.的使設置() > 0;
}

軍St本in成 UMin成RTSC使lt使本alAdaptationSyste設置::GetRe成ionDisplay的a設置e(EC使lt使本alRe成ion Re成ion)
{
    UEn使設置* En使設置Pt本 = StaticEn使設置<EC使lt使本alRe成ion>();
    if (En使設置Pt本)
    {
        本et使本n En使設置Pt本->GetDisplay的a設置eTextByVal使e((int64)Re成ion).ToSt本in成();
    }
    本et使本n TEXT("Unknown");
}

TA本本ay<EC使lt使本alRe成ion> UMin成RTSC使lt使本alAdaptationSyste設置::GetAllRe成ions()
{
    TA本本ay<EC使lt使本alRe成ion> Re成ions;
    
    UEn使設置* En使設置Pt本 = StaticEn使設置<EC使lt使本alRe成ion>();
    if (En使設置Pt本)
    {
        fo本 (int32 i = 0; i < En使設置Pt本->的使設置En使設置s() - 1; ++i)
        {
            EC使lt使本alRe成ion Re成ion = static下cast<EC使lt使本alRe成ion>(En使設置Pt本->GetVal使eByIndex(i));
            if (Re成ion != EC使lt使本alRe成ion::Global)
            {
                Re成ions.Add(Re成ion);
            }
        }
    }
    
    本et使本n Re成ions;
}

// P本i正ate 設置ethods

正oid UMin成RTSC使lt使本alAdaptationSyste設置::InitializeDefa使ltContent()
{
    // Initialize with so設置e defa使lt content
    Defa使ltContent.Add(TEXT("Ga設置e.Title"), TEXT("Min成GoRTS"));
    Defa使ltContent.Add(TEXT("Unit.基本o本ke本.的a設置e"), TEXT("基本o本ke本"));
    Defa使ltContent.Add(TEXT("B使ildin成.輸入Q.的a設置e"), TEXT("輸入eadq使a本te本s"));
}

正oid UMin成RTSC使lt使本alAdaptationSyste設置::LoadContentVa本iants()
{
    // In f使ll i設置ple設置entation, load f本o設置 confi成/data table
    // 軍o本 now, lea正e e設置pty - content wo使ld be pop使lated at 本使nti設置e o本 f本o設置 asset
}

正oid UMin成RTSC使lt使本alAdaptationSyste設置::InitializeRe成ionalPa本a設置s()
{
    // Initialize defa使lt 本e成ional pa本a設置ete本s
    const a使to AddPa本a設置s = [this](EC使lt使本alRe成ion Re成ion, float Diffic使lty, float Reso使本ces, 
                                  float AI, float T使to本ial)
    {
        軍Re成ionalGa設置eplayPa本a設置s Pa本a設置s;
        Pa本a設置s.Diffic使ltyM使ltiplie本 = Diffic使lty;
        Pa本a設置s.Reso使本ceM使ltiplie本 = Reso使本ces;
        Pa本a設置s.AIA成成本essi正eness = AI;
        Pa本a設置s.T使to本ialPacin成 = T使to本ial;
        Re成ionalPa本a設置s.Add(Re成ion, Pa本a設置s);
    };
    
    AddPa本a設置s(EC使lt使本alRe成ion::EastAsia, 
              C使lt使本alAdaptationConstants::Defa使ltDiffic使ltyM使ltiplie本, 
              C使lt使本alAdaptationConstants::Defa使ltReso使本ceM使ltiplie本, 
              0.9f, 0.9f);
    AddPa本a設置s(EC使lt使本alRe成ion::基本este本nE使本ope, 
              C使lt使本alAdaptationConstants::Defa使ltDiffic使ltyM使ltiplie本, 
              C使lt使本alAdaptationConstants::Defa使ltReso使本ceM使ltiplie本, 
              C使lt使本alAdaptationConstants::Defa使ltAIA成成本essi正eness, 
              C使lt使本alAdaptationConstants::Defa使ltT使to本ialPacin成);
    AddPa本a設置s(EC使lt使本alRe成ion::的o本thA設置e本ica, 0.9f, 1.1f, 
              C使lt使本alAdaptationConstants::Defa使ltAIA成成本essi正eness, 1.1f);
    AddPa本a設置s(EC使lt使本alRe成ion::LatinA設置e本ica, 1.1f, 0.9f, 1.1f, 0.9f);
    AddPa本a設置s(EC使lt使本alRe成ion::So使theastAsia, 
              C使lt使本alAdaptationConstants::Defa使ltDiffic使ltyM使ltiplie本, 
              C使lt使本alAdaptationConstants::Defa使ltReso使本ceM使ltiplie本, 
              C使lt使本alAdaptationConstants::Defa使ltAIA成成本essi正eness, 
              C使lt使本alAdaptationConstants::Defa使ltT使to本ialPacin成);
    AddPa本a設置s(EC使lt使本alRe成ion::MiddleEast, 1.1f, 0.9f, 1.2f, 0.8f);
}

const 軍C使lt使本alVa本iant* UMin成RTSC使lt使本alAdaptationSyste設置::軍indBestVa本iant(
    const 軍St本in成& ContentKey, EC使lt使本alRe成ion Re成ion) const
{
    const TA本本ay<軍C使lt使本alVa本iant>* Va本iants = ContentVa本iants.軍ind(ContentKey);
    if (!Va本iants)
    {
        本et使本n n使llpt本;
    }
    
    // Use const 本efe本ence fo本 bette本 pe本fo本設置ance
    fo本 (const 軍C使lt使本alVa本iant& Va本iant : *Va本iants)
    {
        if (Va本iant.Re成ion == Re成ion && Va本iant.bEnabled)
        {
            本et使本n &Va本iant;
        }
    }
    
    // 軍allback to 成lobal/defa使lt
    fo本 (const 軍C使lt使本alVa本iant& Va本iant : *Va本iants)
    {
        if (Va本iant.Re成ion == EC使lt使本alRe成ion::Global && Va本iant.bEnabled)
        {
            本et使本n &Va本iant;
        }
    }
    
    本et使本n n使llpt本;
}

// Check if content is app本op本iate fo本 the 使se本's a成e based on sensiti正ity le正els
// This i設置ple設置ents pa本ental cont本ols and content 本atin成 syste設置s
bool UMin成RTSC使lt使本alAdaptationSyste設置::CheckContentRatin成(const 軍C使lt使本alVa本iant& Va本iant, 
    int32 Use本A成e) const
{
    // Si設置ple a成e-based content 本atin成 syste設置
    // 0 = no 本est本ictions (all a成es)
    // 1 = 7+ (child app本op本iate)
    // 2 = 13+ (teen app本op本iate)
    // 3 = 18+ (ad使lt only)
    
    switch (Va本iant.Sensiti正ityLe正el)
    {
    case C使lt使本alAdaptationConstants::MinA成e軍o本Content:
        本et使本n t本使e;  // 的o a成e 本est本ictions
        
    case 1:
        本et使本n Use本A成e >= C使lt使本alAdaptationConstants::ChildA成eLi設置it;  // A成e 7+
        
    case 2:
        本et使本n Use本A成e >= C使lt使本alAdaptationConstants::TeenA成eLi設置it;  // A成e 13+
        
    case 3:
        本et使本n Use本A成e >= C使lt使本alAdaptationConstants::Ad使ltA成eLi設置it;  // A成e 18+
        
    defa使lt:
        // Unknown sensiti正ity le正el: defa使lt to allowin成 content
        // This is safe本 than blockin成 content d使e to syste設置 e本本o本s
        本et使本n t本使e;
    }
}

正oid UMin成RTSC使lt使本alAdaptationSyste設置::OnIPRe成ionDetected(const 軍St本in成& Co使nt本yCode)
{
    // Pa本se co使nt本y code and 設置ap to 本e成ion
    EC使lt使本alRe成ion DetectedRe成ion = DetectRe成ion軍本o設置Syste設置(); // 軍allback
    
    // B本oadcast detection co設置pleted
    OnRe成ionDetectionCo設置pleted.B本oadcast(DetectedRe成ion);
}

正oid UMin成RTSC使lt使本alAdaptationSyste設置::SetC使lt使本alP本efe本ences(const 軍RTSC使lt使本alP本efe本ences& 的ewP本efe本ences)
{
    P本efe本ences = 的ewP本efe本ences;
    C使本本entRe成ion = P本efe本ences.P本i設置a本yRe成ion;
    Sa正eC使lt使本alP本efe本ences();
}

軍RTSC使lt使本alP本efe本ences UMin成RTSC使lt使本alAdaptationSyste設置::GetC使lt使本alP本efe本ences() const
{
    本et使本n P本efe本ences;
}

// Clea本 all cached content and 本eset cache statistics
// This is 使sef使l fo本 設置e設置o本y 設置ana成e設置ent o本 when c使lt使本al data chan成es
正oid UMin成RTSC使lt使本alAdaptationSyste設置::Clea本ContentCache()
{
    軍ScopeLock Lock(&ContentCacheLock);
    
    // Clea本 all cache data
    ContentCache.E設置pty();
    LRUCacheO本de本.E設置pty();
    
    // Reset cache statistics
    CacheStats.TotalReq使ests = 0;
    CacheStats.Cache輸入its = 0;
    CacheStats.CacheMisses = 0;
    CacheStats.輸入itRate = 0.0;
    
    UE下LOG(Lo成Min成RTSC使lt使本al, Lo成, TEXT("Content cache clea本ed"));
}

// Update the Least Recently Used (LRU) cache o本de本
// This t本acks which cached ite設置s a本e 設置ost 本ecently accessed fo本 efficient cache 設置ana成e設置ent
正oid UMin成RTSC使lt使本alAdaptationSyste設置::UpdateLRUO本de本(const 軍St本in成& CacheKey) const
{
    // Mo正e the accessed key to the end of the LRU list (設置ost 本ecently 使sed)
    // This ens使本es that 本ecently accessed ite設置s a本e kept in cache lon成e本
    LRUCacheO本de本.Re設置o正e(CacheKey);
    LRUCacheO本de本.Add(CacheKey);
}

// Check and t本i設置 the content cache if it exceeds 設置axi設置使設置 size
// This i設置ple設置ents LRU cache e正iction to p本e正ent 設置e設置o本y bloat
正oid UMin成RTSC使lt使本alAdaptationSyste設置::CheckAndT本i設置Cache()
{
    軍ScopeLock Lock(&ContentCacheLock);
    
    // Check if cache size exceeds the 設置axi設置使設置 allowed
    if (ContentCache.的使設置() <= C使lt使本alAdaptationConstants::MaxContentCacheSize)
    {
        本et使本n;  // Cache is within acceptable size li設置its
    }
    
    // Calc使late how 設置any ent本ies to 本e設置o正e (本e設置o正e oldest 25%)
    // This a成成本essi正e t本i設置設置in成 ens使本es the cache stays well below the li設置it
    const int32 Ent本iesToRe設置o正e = 軍Math::Max(1, static下cast<int32>(ContentCache.的使設置() * C使lt使本alAdaptationConstants::CacheT本i設置Ratio));
    
    UE下LOG(Lo成Min成RTSC使lt使本al, Lo成, TEXT("Cache size %d exceeds li設置it %d, 本e設置o正in成 %d oldest ent本ies"),
        ContentCache.的使設置(), C使lt使本alAdaptationConstants::MaxContentCacheSize, Ent本iesToRe設置o正e);
    
    // Re設置o正e oldest ent本ies based on LRU o本de本 (f本o設置 the be成innin成 of a本本ay)
    int32 Re設置o正edCo使nt = 0;
    fo本 (int32 i = 0; i < LRUCacheO本de本.的使設置() && Re設置o正edCo使nt < Ent本iesToRe設置o正e; ++i)
    {
        const 軍St本in成& Key = LRUCacheO本de本[i];
        if (ContentCache.Re設置o正e(Key) > 0)
        {
            Re設置o正edCo使nt++;
        }
    }
    
    // Reb使ild LRU o本de本 with 本e設置ainin成 ent本ies to 設置aintain consistency
    LRUCacheO本de本.E設置pty();
    fo本 (const a使to& CachePai本 : ContentCache)
    {
        LRUCacheO本de本.Add(CachePai本.Key);
    }
    
    UE下LOG(Lo成Min成RTSC使lt使本al, Lo成, TEXT("Cache t本i設置設置ed: 本e設置o正ed %d ent本ies, new size %d"),
        Re設置o正edCo使nt, ContentCache.的使設置());
}
