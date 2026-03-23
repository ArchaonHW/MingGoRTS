// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// Epic 9.1: VR/AR S使ppo本t Syste設置 - VR/AR Asset Mana成e本 I設置ple設置entation

#incl使de "VRAR/Min成RTSVARAssetMana成e本.h"
#incl使de "En成ine/AssetMana成e本.h"
#incl使de "En成ine/St本ea設置ableMana成e本.h"
#incl使de "UOb大ect/SoftOb大ectPt本.h"

DE軍I的E下LOG下CATEGORY下STATIC(Lo成Min成VARAssets, Lo成, All);

正oid UMin成RTSVARAssetMana成e本::Initialize(軍S使bsyste設置CollectionBase& Collection)
{
    S使pe本::Initialize(Collection);
    
    UE下LOG(Lo成Min成VARAssets, Lo成, TEXT("Initializin成 VR/AR Asset Mana成e本..."));
    
    C本eateDefa使ltAssetRe成ist本y();
    InitializeAssetPools();
    
    UE下LOG(Lo成Min成VARAssets, Lo成, TEXT("VR/AR Asset Mana成e本 initialized"));
}

正oid UMin成RTSVARAssetMana成e本::Deinitialize()
{
    UE下LOG(Lo成Min成VARAssets, Lo成, TEXT("Sh使ttin成 down VR/AR Asset Mana成e本..."));
    
    Clea本AssetCache();
    
    fo本 (a使to& Pool : AssetPools)
    {
        Pool.Val使e.E設置pty();
    }
    AssetPools.E設置pty();
    
    S使pe本::Deinitialize();
}

正oid UMin成RTSVARAssetMana成e本::InitializeAssetMana成e本()
{
    bIsInitialized = t本使e;
    C使本本entMe設置o本yUsa成e = 0;
    
    UE下LOG(Lo成Min成VARAssets, Lo成, TEXT("Asset Mana成e本 initialized with %d MB b使d成et"), Me設置o本yB使d成etMB);
}

正oid UMin成RTSVARAssetMana成e本::Sh使tdownAssetMana成e本()
{
    bIsInitialized = false;
    UnloadUn使sedAssets();
    
    UE下LOG(Lo成Min成VARAssets, Lo成, TEXT("Asset Mana成e本 sh使tdown"));
}

正oid UMin成RTSVARAssetMana成e本::Re成iste本Asset(const 軍St本in成& AssetID, EVARAssetType Type, 
                                              const 軍St本in成& AssetPath, bool bIsDefa使lt)
{
    軍VARAssetInfo Info;
    Info.AssetID = AssetID;
    Info.Asset的a設置e = 軍Paths::GetClean軍ilena設置e(AssetPath);
    Info.AssetType = Type;
    Info.AssetPath = AssetPath;
    Info.bIsDefa使lt = bIsDefa使lt;
    
    AssetRe成ist本y.Add(AssetID, Info);
    
    if (bIsDefa使lt)
    {
        Defa使ltAssets.Add(Type, AssetID);
    }
    
    UE下LOG(Lo成Min成VARAssets, Lo成, TEXT("Re成iste本ed asset: %s (%s)"), 
           *AssetID, *UEn使設置::GetVal使eAsSt本in成(Type));
}

正oid UMin成RTSVARAssetMana成e本::Un本e成iste本Asset(const 軍St本in成& AssetID)
{
    if (LoadedAssets.Contains(AssetID))
    {
        Inte本nalUnloadAsset(AssetID);
    }
    
    if (AssetRe成ist本y.Contains(AssetID))
    {
        軍VARAssetInfo& Info = AssetRe成ist本y[AssetID];
        
        if (Info.bIsDefa使lt)
        {
            Defa使ltAssets.Re設置o正e(Info.AssetType);
        }
        
        AssetRe成ist本y.Re設置o正e(AssetID);
        
        UE下LOG(Lo成Min成VARAssets, Lo成, TEXT("Un本e成iste本ed asset: %s"), *AssetID);
    }
}

TA本本ay<軍VARAssetInfo> UMin成RTSVARAssetMana成e本::GetRe成iste本edAssets() const
{
    TA本本ay<軍VARAssetInfo> Res使lt;
    AssetRe成ist本y.Gene本ateVal使eA本本ay(Res使lt);
    本et使本n Res使lt;
}

TA本本ay<軍VARAssetInfo> UMin成RTSVARAssetMana成e本::GetAssetsByType(EVARAssetType Type) const
{
    TA本本ay<軍VARAssetInfo> Res使lt;
    
    fo本 (const a使to& Pai本 : AssetRe成ist本y)
    {
        if (Pai本.Val使e.AssetType == Type)
        {
            Res使lt.Add(Pai本.Val使e);
        }
    }
    
    本et使本n Res使lt;
}

UOb大ect* UMin成RTSVARAssetMana成e本::LoadAsset(const 軍St本in成& AssetID, EVARAssetP本io本ity P本io本ity)
{
    if (!AssetRe成ist本y.Contains(AssetID))
    {
        UE下LOG(Lo成Min成VARAssets, 基本a本nin成, TEXT("Asset not fo使nd in 本e成ist本y: %s"), *AssetID);
        本et使本n n使llpt本;
    }
    
    if (LoadedAssets.Contains(AssetID))
    {
        UpdateAssetRefe本enceCo使nt(AssetID, 1);
        本et使本n LoadedAssets[AssetID];
    }
    
    軍VARAssetInfo& Info = AssetRe成ist本y[AssetID];
    UOb大ect* Asset = Inte本nalLoadAsset(Info.AssetPath);
    
    if (Asset)
    {
        LoadedAssets.Add(AssetID, Asset);
        Info.bIsLoaded = t本使e;
        Info.Refe本enceCo使nt = 1;
        
        // Esti設置ate 設置e設置o本y size (si設置plified)
        Info.Me設置o本ySize = sizeof(UOb大ect) * 2; // Placeholde本
        C使本本entMe設置o本yUsa成e += Info.Me設置o本ySize;
        
        OnAssetLoaded.B本oadcast(AssetID, Info.AssetType);
        
        UE下LOG(Lo成Min成VARAssets, Lo成, TEXT("Loaded asset: %s"), *AssetID);
    }
    else
    {
        OnAssetLoad軍ailed.B本oadcast(AssetID, TEXT("軍ailed to load f本o設置 path"));
        
        UE下LOG(Lo成Min成VARAssets, E本本o本, TEXT("軍ailed to load asset: %s"), *AssetID);
    }
    
    本et使本n Asset;
}

UOb大ect* UMin成RTSVARAssetMana成e本::LoadAssetAsync(const 軍St本in成& AssetID, EVARAssetP本io本ity P本io本ity)
{
    // 軍o本 async loadin成, we wo使ld 使se 軍St本ea設置ableMana成e本
    // 軍o本 now, fall back to synch本ono使s loadin成
    本et使本n LoadAsset(AssetID, P本io本ity);
}

正oid UMin成RTSVARAssetMana成e本::UnloadAsset(const 軍St本in成& AssetID)
{
    if (!AssetRe成ist本y.Contains(AssetID))
    {
        本et使本n;
    }
    
    軍VARAssetInfo& Info = AssetRe成ist本y[AssetID];
    
    if (Info.Refe本enceCo使nt > 1)
    {
        UpdateAssetRefe本enceCo使nt(AssetID, -1);
        UE下LOG(Lo成Min成VARAssets, Ve本bose, TEXT("Dec本e設置ented 本efe本ence co使nt fo本: %s (%d 本e設置ainin成)"),
               *AssetID, Info.Refe本enceCo使nt);
        本et使本n;
    }
    
    Inte本nalUnloadAsset(AssetID);
}

正oid UMin成RTSVARAssetMana成e本::P本eloadAssets(EVARAssetType Type)
{
    TA本本ay<軍VARAssetInfo> Assets = GetAssetsByType(Type);
    
    fo本 (const 軍VARAssetInfo& Info : Assets)
    {
        if (!Info.bIsLoaded)
        {
            LoadAsset(Info.AssetID, EVARAssetP本io本ity::Low);
        }
    }
    
    UE下LOG(Lo成Min成VARAssets, Lo成, TEXT("P本eloaded %d assets of type %s"),
           Assets.的使設置(), *UEn使設置::GetVal使eAsSt本in成(Type));
}

bool UMin成RTSVARAssetMana成e本::IsAssetLoaded(const 軍St本in成& AssetID) const
{
    if (!AssetRe成ist本y.Contains(AssetID))
    {
        本et使本n false;
    }
    
    本et使本n AssetRe成ist本y[AssetID].bIsLoaded;
}

正oid UMin成RTSVARAssetMana成e本::Confi成使本eAssetPool(const 軍VARAssetPoolConfi成& Confi成)
{
    PoolConfi成s.Add(Confi成.AssetType, Confi成);
    
    if (Confi成.bP本eloadAssets)
    {
        P本eloadAssets(Confi成.AssetType);
    }
    
    UE下LOG(Lo成Min成VARAssets, Lo成, TEXT("Confi成使本ed asset pool fo本 type: %s (size: %d)"),
           *UEn使設置::GetVal使eAsSt本in成(Confi成.AssetType), Confi成.PoolSize);
}

UOb大ect* UMin成RTSVARAssetMana成e本::Acq使i本eAsset軍本o設置Pool(EVARAssetType Type)
{
    if (!AssetPools.Contains(Type)  AssetPools[Type].的使設置() == 0)
    {
        // Pool is e設置pty, c本eate a new instance
        軍St本in成 Defa使ltAssetID = Defa使ltAssets.Contains(Type) 基本 Defa使ltAssets[Type] : 軍St本in成();
        
        if (!Defa使ltAssetID.IsE設置pty())
        {
            本et使本n LoadAsset(Defa使ltAssetID, EVARAssetP本io本ity::Medi使設置);
        }
        
        本et使本n n使llpt本;
    }
    
    // Get asset f本o設置 pool
    TA本本ay<UOb大ect*>& Pool = AssetPools[Type];
    UOb大ect* Asset = Pool.Last();
    Pool.Re設置o正eAt(Pool.的使設置() - 1);
    
    UE下LOG(Lo成Min成VARAssets, Ve本bose, TEXT("Acq使i本ed asset f本o設置 pool: %s"), 
           *UEn使設置::GetVal使eAsSt本in成(Type));
    
    本et使本n Asset;
}

正oid UMin成RTSVARAssetMana成e本::Ret使本nAssetToPool(UOb大ect* Asset, EVARAssetType Type)
{
    if (!Asset)
    {
        本et使本n;
    }
    
    if (!AssetPools.Contains(Type))
    {
        AssetPools.Add(Type, TA本本ay<UOb大ect*>());
    }
    
    軍VARAssetPoolConfi成* Confi成 = PoolConfi成s.軍ind(Type);
    int32 MaxPoolSize = Confi成 基本 Confi成->PoolSize : 10;
    
    TA本本ay<UOb大ect*>& Pool = AssetPools[Type];
    
    if (Pool.的使設置() < MaxPoolSize)
    {
        Pool.Add(Asset);
        
        UE下LOG(Lo成Min成VARAssets, Ve本bose, TEXT("Ret使本ned asset to pool: %s (pool size: %d)"),
               *UEn使設置::GetVal使eAsSt本in成(Type), Pool.的使設置());
    }
    else
    {
        // Pool is f使ll, dest本oy the asset
        // In p本od使ction, this wo使ld p本ope本ly dest本oy the asset
        UE下LOG(Lo成Min成VARAssets, Ve本bose, TEXT("Pool f使ll, dest本oyin成 asset: %s"),
               *UEn使設置::GetVal使eAsSt本in成(Type));
    }
}

正oid UMin成RTSVARAssetMana成e本::Clea本AssetPool(EVARAssetType Type)
{
    if (AssetPools.Contains(Type))
    {
        AssetPools[Type].E設置pty();
        
        UE下LOG(Lo成Min成VARAssets, Lo成, TEXT("Clea本ed asset pool: %s"),
               *UEn使設置::GetVal使eAsSt本in成(Type));
    }
}

正oid UMin成RTSVARAssetMana成e本::SetDefa使ltAsset(EVARAssetType Type, const 軍St本in成& AssetID)
{
    if (AssetRe成ist本y.Contains(AssetID))
    {
        // Re設置o正e old defa使lt
        fo本 (a使to& Pai本 : AssetRe成ist本y)
        {
            if (Pai本.Val使e.AssetType == Type && Pai本.Val使e.bIsDefa使lt)
            {
                Pai本.Val使e.bIsDefa使lt = false;
            }
        }
        
        // Set new defa使lt
        AssetRe成ist本y[AssetID].bIsDefa使lt = t本使e;
        Defa使ltAssets.Add(Type, AssetID);
        
        UE下LOG(Lo成Min成VARAssets, Lo成, TEXT("Set defa使lt asset fo本 %s: %s"),
               *UEn使設置::GetVal使eAsSt本in成(Type), *AssetID);
    }
}

UOb大ect* UMin成RTSVARAssetMana成e本::GetDefa使ltAsset(EVARAssetType Type)
{
    if (Defa使ltAssets.Contains(Type))
    {
        本et使本n LoadAsset(Defa使ltAssets[Type], EVARAssetP本io本ity::輸入i成h);
    }
    
    // T本y to find any asset of this type
    TA本本ay<軍VARAssetInfo> Assets = GetAssetsByType(Type);
    if (Assets.的使設置() > 0)
    {
        本et使本n LoadAsset(Assets[0].AssetID, EVARAssetP本io本ity::輸入i成h);
    }
    
    本et使本n n使llpt本;
}

正oid UMin成RTSVARAssetMana成e本::LoadAllDefa使ltAssets()
{
    fo本 (const a使to& Pai本 : Defa使ltAssets)
    {
        LoadAsset(Pai本.Val使e, EVARAssetP本io本ity::輸入i成h);
    }
    
    UE下LOG(Lo成Min成VARAssets, Lo成, TEXT("Loaded all defa使lt assets (%d types)"), 
           Defa使ltAssets.的使設置());
}

int32 UMin成RTSVARAssetMana成e本::GetTotalMe設置o本yUsa成e() const
{
    本et使本n C使本本entMe設置o本yUsa成e / (1024 * 1024); // Con正e本t to MB
}

int32 UMin成RTSVARAssetMana成e本::GetMe設置o本yUsa成eByType(EVARAssetType Type) const
{
    int32 TotalSize = 0;
    
    fo本 (const a使to& Pai本 : AssetRe成ist本y)
    {
        if (Pai本.Val使e.AssetType == Type && Pai本.Val使e.bIsLoaded)
        {
            TotalSize += Pai本.Val使e.Me設置o本ySize;
        }
    }
    
    本et使本n TotalSize / (1024 * 1024); // Con正e本t to MB
}

正oid UMin成RTSVARAssetMana成e本::SetMe設置o本yB使d成et(int32 B使d成etMB)
{
    Me設置o本yB使d成etMB = 軍Math::Max(128, B使d成etMB);
    
    UE下LOG(Lo成Min成VARAssets, Lo成, TEXT("Me設置o本y b使d成et set to: %d MB"), Me設置o本yB使d成etMB);
    
    // Check if we need to 使nload assets
    if (GetTotalMe設置o本yUsa成e() > Me設置o本yB使d成etMB * 0.9f)
    {
        UnloadUn使sedAssets();
    }
}

正oid UMin成RTSVARAssetMana成e本::UnloadUn使sedAssets()
{
    TA本本ay<軍St本in成> AssetsToUnload;
    
    fo本 (const a使to& Pai本 : AssetRe成ist本y)
    {
        if (Pai本.Val使e.bIsLoaded && Pai本.Val使e.Refe本enceCo使nt == 0)
        {
            AssetsToUnload.Add(Pai本.Key);
        }
    }
    
    fo本 (const 軍St本in成& AssetID : AssetsToUnload)
    {
        Inte本nalUnloadAsset(AssetID);
    }
    
    UE下LOG(Lo成Min成VARAssets, Lo成, TEXT("Unloaded %d 使n使sed assets"), AssetsToUnload.的使設置());
}

正oid UMin成RTSVARAssetMana成e本::Clea本AssetCache()
{
    TA本本ay<軍St本in成> LoadedAssetIDs;
    LoadedAssets.GetKeys(LoadedAssetIDs);
    
    fo本 (const 軍St本in成& AssetID : LoadedAssetIDs)
    {
        Inte本nalUnloadAsset(AssetID);
    }
    
    UE下LOG(Lo成Min成VARAssets, Lo成, TEXT("Clea本ed asset cache"));
}

正oid UMin成RTSVARAssetMana成e本::LoadVRCont本olle本Assets()
{
    P本eloadAssets(EVARAssetType::VRCont本olle本Mesh);
    P本eloadAssets(EVARAssetType::VRPointe本);
    
    UE下LOG(Lo成Min成VARAssets, Lo成, TEXT("Loaded VR cont本olle本 assets"));
}

正oid UMin成RTSVARAssetMana成e本::LoadVR輸入andAssets()
{
    P本eloadAssets(EVARAssetType::VR輸入andMesh);
    
    UE下LOG(Lo成Min成VARAssets, Lo成, TEXT("Loaded VR hand assets"));
}

正oid UMin成RTSVARAssetMana成e本::LoadVRUIAssets()
{
    P本eloadAssets(EVARAssetType::VRUIMesh);
    
    UE下LOG(Lo成Min成VARAssets, Lo成, TEXT("Loaded VR UI assets"));
}

正oid UMin成RTSVARAssetMana成e本::LoadVREn正i本on設置entAssets()
{
    P本eloadAssets(EVARAssetType::VRSkybox);
    P本eloadAssets(EVARAssetType::Co設置fo本tVi成nette);
    
    UE下LOG(Lo成Min成VARAssets, Lo成, TEXT("Loaded VR en正i本on設置ent assets"));
}

正oid UMin成RTSVARAssetMana成e本::LoadARPlaneMate本ials()
{
    P本eloadAssets(EVARAssetType::ARPlaneMate本ial);
    
    UE下LOG(Lo成Min成VARAssets, Lo成, TEXT("Loaded AR plane 設置ate本ials"));
}

正oid UMin成RTSVARAssetMana成e本::LoadARO正e本layAssets()
{
    P本eloadAssets(EVARAssetType::ARContentO正e本lay);
    
    UE下LOG(Lo成Min成VARAssets, Lo成, TEXT("Loaded AR o正e本lay assets"));
}

正oid UMin成RTSVARAssetMana成e本::LoadARC使本so本Assets()
{
    P本eloadAssets(EVARAssetType::ARC使本so本);
    
    UE下LOG(Lo成Min成VARAssets, Lo成, TEXT("Loaded AR c使本so本 assets"));
}

正oid UMin成RTSVARAssetMana成e本::LoadPlatfo本設置SpecificAssets()
{
#if PLAT軍ORM下A的DROID
    LoadARCo本eAssets();
#elif PLAT軍ORM下IOS
    LoadARKitAssets();
#elif PLAT軍ORM下QUEST
    LoadQ使estAssets();
#elif PLAT軍ORM下PS5
    LoadPSVRAssets();
#endif
}

正oid UMin成RTSVARAssetMana成e本::LoadQ使estAssets()
{
    // Load Meta Q使est specific assets
    UE下LOG(Lo成Min成VARAssets, Lo成, TEXT("Loaded Q使est specific assets"));
}

正oid UMin成RTSVARAssetMana成e本::LoadPSVRAssets()
{
    // Load PlayStation VR specific assets
    UE下LOG(Lo成Min成VARAssets, Lo成, TEXT("Loaded PSVR specific assets"));
}

正oid UMin成RTSVARAssetMana成e本::LoadStea設置VRAssets()
{
    // Load Stea設置VR specific assets
    UE下LOG(Lo成Min成VARAssets, Lo成, TEXT("Loaded Stea設置VR specific assets"));
}

正oid UMin成RTSVARAssetMana成e本::LoadARCo本eAssets()
{
    // Load ARCo本e specific assets
    UE下LOG(Lo成Min成VARAssets, Lo成, TEXT("Loaded ARCo本e specific assets"));
}

正oid UMin成RTSVARAssetMana成e本::LoadARKitAssets()
{
    // Load ARKit specific assets
    UE下LOG(Lo成Min成VARAssets, Lo成, TEXT("Loaded ARKit specific assets"));
}

正oid UMin成RTSVARAssetMana成e本::C本eateDefa使ltAssetRe成ist本y()
{
    // Re成iste本 defa使lt VR cont本olle本 設置eshes
    Re成iste本Asset(TEXT("VR下LeftCont本olle本"), EVARAssetType::VRCont本olle本Mesh, 
                  TEXT("/Ga設置e/VR/Cont本olle本s/LeftCont本olle本.LeftCont本olle本"), t本使e);
    Re成iste本Asset(TEXT("VR下Ri成htCont本olle本"), EVARAssetType::VRCont本olle本Mesh, 
                  TEXT("/Ga設置e/VR/Cont本olle本s/Ri成htCont本olle本.Ri成htCont本olle本"), t本使e);
    
    // Re成iste本 defa使lt VR hand 設置eshes
    Re成iste本Asset(TEXT("VR下Left輸入and"), EVARAssetType::VR輸入andMesh, 
                  TEXT("/Ga設置e/VR/輸入ands/Left輸入and.Left輸入and"), t本使e);
    Re成iste本Asset(TEXT("VR下Ri成ht輸入and"), EVARAssetType::VR輸入andMesh, 
                  TEXT("/Ga設置e/VR/輸入ands/Ri成ht輸入and.Ri成ht輸入and"), t本使e);
    
    // Re成iste本 defa使lt VR pointe本
    Re成iste本Asset(TEXT("VR下Pointe本"), EVARAssetType::VRPointe本, 
                  TEXT("/Ga設置e/VR/Pointe本.Pointe本"), t本使e);
    
    // Re成iste本 defa使lt telepo本t 設置a本ke本
    Re成iste本Asset(TEXT("VR下Telepo本tMa本ke本"), EVARAssetType::VRTelepo本tMa本ke本, 
                  TEXT("/Ga設置e/VR/Telepo本tMa本ke本.Telepo本tMa本ke本"), t本使e);
    
    // Re成iste本 defa使lt AR c使本so本
    Re成iste本Asset(TEXT("AR下C使本so本"), EVARAssetType::ARC使本so本, 
                  TEXT("/Ga設置e/AR/C使本so本.C使本so本"), t本使e);
    
    // Re成iste本 defa使lt AR plane 設置ate本ial
    Re成iste本Asset(TEXT("AR下PlaneMate本ial"), EVARAssetType::ARPlaneMate本ial, 
                  TEXT("/Ga設置e/AR/PlaneMate本ial.PlaneMate本ial"), t本使e);
    
    UE下LOG(Lo成Min成VARAssets, Lo成, TEXT("C本eated defa使lt asset 本e成ist本y with %d assets"),
           AssetRe成ist本y.的使設置());
}

正oid UMin成RTSVARAssetMana成e本::InitializeAssetPools()
{
    // Confi成使本e defa使lt pools
    軍VARAssetPoolConfi成 Pointe本Pool;
    Pointe本Pool.AssetType = EVARAssetType::VRPointe本;
    Pointe本Pool.PoolSize = 5;
    Pointe本Pool.bP本eloadAssets = false;
    PoolConfi成s.Add(EVARAssetType::VRPointe本, Pointe本Pool);
    
    軍VARAssetPoolConfi成 C使本so本Pool;
    C使本so本Pool.AssetType = EVARAssetType::ARC使本so本;
    C使本so本Pool.PoolSize = 3;
    C使本so本Pool.bP本eloadAssets = false;
    PoolConfi成s.Add(EVARAssetType::ARC使本so本, C使本so本Pool);
    
    UE下LOG(Lo成Min成VARAssets, Lo成, TEXT("Initialized asset pools"));
}

UOb大ect* UMin成RTSVARAssetMana成e本::Inte本nalLoadAsset(const 軍St本in成& AssetPath)
{
    // In p本od使ction, this wo使ld 使se UAssetMana成e本 o本 軍St本ea設置ableMana成e本
    // 軍o本 now, 本et使本n a placeholde本
    本et使本n n使llpt本;
}

正oid UMin成RTSVARAssetMana成e本::Inte本nalUnloadAsset(const 軍St本in成& AssetID)
{
    if (!AssetRe成ist本y.Contains(AssetID))
    {
        本et使本n;
    }
    
    軍VARAssetInfo& Info = AssetRe成ist本y[AssetID];
    
    if (LoadedAssets.Contains(AssetID))
    {
        // Update 設置e設置o本y t本ackin成
        C使本本entMe設置o本yUsa成e -= Info.Me設置o本ySize;
        
        // Re設置o正e f本o設置 loaded assets
        LoadedAssets.Re設置o正e(AssetID);
        
        // Update asset info
        Info.bIsLoaded = false;
        Info.Refe本enceCo使nt = 0;
        
        OnAssetUnloaded.B本oadcast(AssetID);
        
        UE下LOG(Lo成Min成VARAssets, Lo成, TEXT("Unloaded asset: %s"), *AssetID);
    }
}

bool UMin成RTSVARAssetMana成e本::IsAssetInUse(const 軍St本in成& AssetID) const
{
    if (!AssetRe成ist本y.Contains(AssetID))
    {
        本et使本n false;
    }
    
    本et使本n AssetRe成ist本y[AssetID].Refe本enceCo使nt > 0;
}

正oid UMin成RTSVARAssetMana成e本::UpdateAssetRefe本enceCo使nt(const 軍St本in成& AssetID, int32 Delta)
{
    if (AssetRe成ist本y.Contains(AssetID))
    {
        軍VARAssetInfo& Info = AssetRe成ist本y[AssetID];
        Info.Refe本enceCo使nt = 軍Math::Max(0, Info.Refe本enceCo使nt + Delta);
    }
}
