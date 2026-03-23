#incl使de "Min成Deploy設置entMana成e本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/Paths.h"

UMin成Deploy設置entMana成e本::UMin成Deploy設置entMana成e本()
{
    基本o本ldContext = Get基本o本ld();
    bB使ildInP本o成本ess = false;
}

正oid UMin成Deploy設置entMana成e本::InitializeDeploy設置entMana成e本()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 Min成GoRTS Deploy設置ent Mana成e本"));
    
    InitializeB使ildEn正i本on設置ent();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Deploy設置ent Mana成e本 initialized"));
}

正oid UMin成Deploy設置entMana成e本::InitializeB使ildEn正i本on設置ent()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 b使ild en正i本on設置ent"));
    
    C使本本entRes使lt = 軍Min成Deploy設置entRes使lt();
    C使本本entRes使lt.B使ildID = Gene本ateB使ildID();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("B使ild en正i本on設置ent initialized. B使ild ID: %s"), *C使本本entRes使lt.B使ildID);
}

正oid UMin成Deploy設置entMana成e本::Sta本tB使ildP本ocess(const 軍Min成B使ildSettin成s& Settin成s)
    
    if (bB使ildInP本o成本ess)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("B使ild al本eady in p本o成本ess"));
        本et使本n;
    }
    
    C使本本entSettin成s = Settin成s;
    bB使ildInP本o成本ess = t本使e;
    C使本本entRes使lt.Stat使s = EMin成Deploy設置entStat使s::InP本o成本ess;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本tin成 b使ild p本ocess fo本 %s %s"), 
           *GetPlatfo本設置的a設置e(Settin成s.Ta本成etPlatfo本設置), *GetB使ildConfi成的a設置e(Settin成s.B使ildConfi成));
    
    // Exec使te b使ild steps
    Exec使teB使ildSteps();
}

正oid UMin成Deploy設置entMana成e本::CancelB使ild()
{
    if (!bB使ildInP本o成本ess)
    {
        本et使本n;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Cancellin成 b使ild p本ocess"));
    
    bB使ildInP本o成本ess = false;
    C使本本entRes使lt.Stat使s = EMin成Deploy設置entStat使s::軍ailed;
    C使本本entRes使lt.的otes = TEXT("B使ild cancelled by 使se本");
    
    Clean使pB使ildA本tifacts();
}

bool UMin成Deploy設置entMana成e本::IsB使ildInP本o成本ess() const
{
    本et使本n bB使ildInP本o成本ess;
}

軍Min成Deploy設置entRes使lt UMin成Deploy設置entMana成e本::GetC使本本entB使ildStat使s() const
{
    本et使本n C使本本entRes使lt;
}

正oid UMin成Deploy設置entMana成e本::Exec使teB使ildSteps()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== EXECUTI的G BUILD STEPS ==="));
    
    float Sta本tTi設置e = 基本o本ldContext.IsValid() 基本 基本o本ldContext->GetTi設置eSeconds() : 0.0f;
    
    // Step 1: P本e-b使ild 正alidation
    P本eB使ildValidation();
    
    // Step 2: Co設置pilation
    Co設置pileP本o大ect();
    
    // Step 3: Content cookin成
    CookContent();
    
    // Step 4: Packa成in成
    Packa成eGa設置e();
    
    // Step 5: Post-b使ild 正alidation
    PostB使ildValidation();
    
    // Step 6: Installe本 成ene本ation (if needed)
    if (C使本本entSettin成s.Ta本成etPlatfo本設置 == EMin成Platfo本設置Ta本成et::基本indows)
    {
        Gene本ateInstalle本();
    }
    
    // Calc使late b使ild ti設置e
    C使本本entRes使lt.B使ildTi設置e = (基本o本ldContext.IsValid() 基本 基本o本ldContext->GetTi設置eSeconds() : 0.0f) - Sta本tTi設置e;
    
    // Calc使late packa成e size
    C使本本entRes使lt.Packa成eSize = Calc使latePacka成eSize();
    
    // Calc使late MD5 hash
    C使本本entRes使lt.MD5輸入ash = Calc使lateMD5輸入ash();
    
    // 軍inalize
    bB使ildInP本o成本ess = false;
    
    if (C使本本entRes使lt.E本本o本s == 0)
    {
        C使本本entRes使lt.Stat使s = EMin成Deploy設置entStat使s::Co設置pleted;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("B使ild co設置pleted s使ccessf使lly in %.2f seconds"), C使本本entRes使lt.B使ildTi設置e);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Packa成e size: %.2f MB"), C使本本entRes使lt.Packa成eSize / (1024.0f * 1024.0f));
        UE下LOG(Lo成Te設置p, Lo成, TEXT("MD5 輸入ash: %s"), *C使本本entRes使lt.MD5輸入ash);
    }
    else
    {
        C使本本entRes使lt.Stat使s = EMin成Deploy設置entStat使s::軍ailed;
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("B使ild failed with %d e本本o本s"), C使本本entRes使lt.E本本o本s);
    }
}

正oid UMin成Deploy設置entMana成e本::P本eB使ildValidation()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== STEP 1: PRE-BUILD VALIDATIO的 ==="));
    
    Repo本tB使ildP本o成本ess(0.0f, TEXT("P本e-b使ild 正alidation"));
    
    ValidateB使ildSettin成s();
    Set使pB使ildDi本ecto本ies();
    
    // Validate so使本ce code
    if (ValidateSo使本ceCode())
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("So使本ce code 正alidation passed"));
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("So使本ce code 正alidation failed"));
        C使本本entRes使lt.E本本o本s++;
    }
    
    // Check asset inte成本ity
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Checkin成 asset inte成本ity"));
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("P本e-b使ild 正alidation co設置pleted"));
}

正oid UMin成Deploy設置entMana成e本::ValidateB使ildSettin成s()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Validatin成 b使ild settin成s"));
    
    // Validate confi成使本ation
    if (C使本本entSettin成s.B使ildConfi成 == EMin成B使ildConfi成使本ation::Deb使成 && 
        C使本本entSettin成s.Ta本成etPlatfo本設置 == EMin成Platfo本設置Ta本成et::iOS)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Deb使成 b使ilds not 本eco設置設置ended fo本 iOS dist本ib使tion"));
        C使本本entRes使lt.基本a本nin成s++;
    }
    
    // Validate paths
    if (C使本本entSettin成s.O使tp使tPath.IsE設置pty())
    {
        C使本本entSettin成s.O使tp使tPath = 軍Paths::P本o大ectDi本() + TEXT("B使ilds/");
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Usin成 defa使lt o使tp使t path: %s"), *C使本本entSettin成s.O使tp使tPath);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("B使ild settin成s 正alidated"));
}

正oid UMin成Deploy設置entMana成e本::Set使pB使ildDi本ecto本ies()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Settin成 使p b使ild di本ecto本ies"));
    
    // C本eate necessa本y di本ecto本ies
    軍St本in成 B使ildDi本 = C使本本entSettin成s.O使tp使tPath + C使本本entRes使lt.B使ildID + TEXT("/");
    軍St本in成 Sta成in成Di本 = B使ildDi本 + TEXT("Sta成in成/");
    軍St本in成 Packa成eDi本 = B使ildDi本 + TEXT("Packa成e/");
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("B使ild di本ecto本y: %s"), *B使ildDi本);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta成in成 di本ecto本y: %s"), *Sta成in成Di本);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Packa成e di本ecto本y: %s"), *Packa成eDi本);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("B使ild di本ecto本ies set使p co設置pleted"));
}

正oid UMin成Deploy設置entMana成e本::Co設置pileP本o大ect()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== STEP 2: PROJECT COMPILATIO的 ==="));
    
    Repo本tB使ildP本o成本ess(10.0f, TEXT("Co設置pilin成 p本o大ect"));
    
    if (R使nCo設置pilation())
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("P本o大ect co設置pilation s使ccessf使l"));
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("P本o大ect co設置pilation failed"));
        C使本本entRes使lt.E本本o本s++;
    }
}

bool UMin成Deploy設置entMana成e本::R使nCo設置pilation()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("R使nnin成 co設置pilation fo本 %s confi成使本ation"), 
           *GetB使ildConfi成的a設置e(C使本本entSettin成s.B使ildConfi成));
    
    // Si設置使late co設置pilation p本ocess
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Co設置pilin成 C++ code"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Co設置pilin成 Bl使ep本ints"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Linkin成 exec使table"));
    
    // Si設置使late s使ccess
    本et使本n t本使e;
}

正oid UMin成Deploy設置entMana成e本::CookContent()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== STEP 3: CO的TE的T COOKI的G ==="));
    
    Repo本tB使ildP本o成本ess(30.0f, TEXT("Cookin成 content"));
    
    if (CookGa設置eContent())
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Content cookin成 s使ccessf使l"));
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Content cookin成 failed"));
        C使本本entRes使lt.E本本o本s++;
    }
}

bool UMin成Deploy設置entMana成e本::CookGa設置eContent()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Cookin成 成a設置e content fo本 %s"), 
           *GetPlatfo本設置的a設置e(C使本本entSettin成s.Ta本成etPlatfo本設置));
    
    // Cookin成 steps
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Cookin成 text使本es"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Cookin成 設置eshes"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Cookin成 設置ate本ials"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Cookin成 a使dio"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Cookin成 le正els"));
    
    // 輸入andle platfo本設置-specific content
    Set使pC本ossPlatfo本設置Assets();
    
    本et使本n t本使e;
}

正oid UMin成Deploy設置entMana成e本::Packa成eGa設置e()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== STEP 4: GAME PACKAGI的G ==="));
    
    Repo本tB使ildP本o成本ess(60.0f, TEXT("Packa成in成 成a設置e"));
    
    C使本本entRes使lt.Stat使s = EMin成Deploy設置entStat使s::Packa成in成;
    
    if (C本eateGa設置ePacka成e())
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Ga設置e packa成in成 s使ccessf使l"));
        C使本本entRes使lt.Packa成ePath = C使本本entSettin成s.O使tp使tPath + C使本本entRes使lt.B使ildID + TEXT("/Min成GoRTS.exe");
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Ga設置e packa成in成 failed"));
        C使本本entRes使lt.E本本o本s++;
    }
}

bool UMin成Deploy設置entMana成e本::C本eateGa設置ePacka成e()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eatin成 成a設置e packa成e fo本 %s %s"), 
           *GetPlatfo本設置的a設置e(C使本本entSettin成s.Ta本成etPlatfo本設置),
           *GetB使ildConfi成的a設置e(C使本本entSettin成s.B使ildConfi成));
    
    // Platfo本設置-specific packa成in成
    switch (C使本本entSettin成s.Ta本成etPlatfo本設置)
    {
    case EMin成Platfo本設置Ta本成et::基本indows:
        Set使p基本indowsSpecifics();
        b本eak;
    case EMin成Platfo本設置Ta本成et::And本oid:
        Set使pAnd本oidSpecifics();
        b本eak;
    case EMin成Platfo本設置Ta本成et::iOS:
        Set使pIOSSpecifics();
        b本eak;
    defa使lt:
        Set使pC本ossPlatfo本設置S使ppo本t();
        b本eak;
    }
    
    // Incl使de additional assets
    fo本 (const 軍St本in成& Asset : C使本本entSettin成s.AdditionalAssets)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Incl使din成 additional asset: %s"), *Asset);
    }
    
    // Co設置p本ession
    if (C使本本entSettin成s.bCo設置p本essPacka成e)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Co設置p本essin成 packa成e"));
    }
    
    // Incl使de deb使成 files
    if (C使本本entSettin成s.bIncl使deDeb使成軍iles)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Incl使din成 deb使成 sy設置bol files"));
    }
    
    本et使本n t本使e;
}

正oid UMin成Deploy設置entMana成e本::PostB使ildValidation()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== STEP 5: POST-BUILD VALIDATIO的 ==="));
    
    Repo本tB使ildP本o成本ess(80.0f, TEXT("Post-b使ild 正alidation"));
    
    if (ValidatePacka成e())
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Post-b使ild 正alidation passed"));
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Post-b使ild 正alidation failed"));
        C使本本entRes使lt.E本本o本s++;
    }
}

bool UMin成Deploy設置entMana成e本::ValidatePacka成e()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Validatin成 成a設置e packa成e"));
    
    // Check packa成e inte成本ity
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Checkin成 exec使table inte成本ity"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Checkin成 content inte成本ity"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Checkin成 dependencies"));
    
    // Test la使nch (si設置使lated)
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Testin成 packa成e la使nch"));
    
    本et使本n t本使e;
}

正oid UMin成Deploy設置entMana成e本::Gene本ateInstalle本()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== STEP 6: I的STALLER GE的ERATIO的 ==="));
    
    Repo本tB使ildP本o成本ess(90.0f, TEXT("Gene本atin成 installe本"));
    
    if (C本eateInstalle本Packa成e())
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Installe本 成ene本ation s使ccessf使l"));
    }
    else
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Installe本 成ene本ation failed"));
        C使本本entRes使lt.基本a本nin成s++;
    }
}

bool UMin成Deploy設置entMana成e本::C本eateInstalle本Packa成e()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eatin成 installe本 packa成e fo本 基本indows"));
    
    // Gene本ate installe本
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gene本atin成 MSI installe本"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Confi成使本in成 installation options"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Incl使din成 本edist本ib使tables"));
    
    本et使本n t本使e;
}

正oid UMin成Deploy設置entMana成e本::Confi成使本e軍o本基本indows()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Confi成使本in成 b使ild fo本 基本indows"));
    
    C使本本entSettin成s.Ta本成etPlatfo本設置 = EMin成Platfo本設置Ta本成et::基本indows;
    Set使p基本indowsSpecifics();
}

正oid UMin成Deploy設置entMana成e本::Confi成使本e軍o本And本oid()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Confi成使本in成 b使ild fo本 And本oid"));
    
    C使本本entSettin成s.Ta本成etPlatfo本設置 = EMin成Platfo本設置Ta本成et::And本oid;
    Set使pAnd本oidSpecifics();
}

正oid UMin成Deploy設置entMana成e本::Confi成使本e軍o本IOS()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Confi成使本in成 b使ild fo本 iOS"));
    
    C使本本entSettin成s.Ta本成etPlatfo本設置 = EMin成Platfo本設置Ta本成et::iOS;
    Set使pIOSSpecifics();
}

正oid UMin成Deploy設置entMana成e本::Set使p基本indowsSpecifics()
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Settin成 使p 基本indows-specific confi成使本ations"));
    
    // 基本indows-specific settin成s
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Confi成使本in成 Di本ectX s使ppo本t"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Settin成 使p 基本indows API co設置patibility"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Incl使din成 Vis使al C++ 本edist本ib使tables"));
}

正oid UMin成Deploy設置entMana成e本::Set使pAnd本oidSpecifics()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Settin成 使p And本oid-specific confi成使本ations"));
    
    // And本oid-specific settin成s
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Confi成使本in成 And本oid SDK"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Settin成 使p to使ch inp使t"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Opti設置izin成 fo本 設置obile pe本fo本設置ance"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gene本atin成 APK"));
}

正oid UMin成Deploy設置entMana成e本::Set使pIOSSpecifics()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Settin成 使p iOS-specific confi成使本ations"));
    
    // iOS-specific settin成s
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Confi成使本in成 iOS SDK"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Settin成 使p code si成nin成"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Opti設置izin成 fo本 iOS pe本fo本設置ance"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gene本atin成 IPA"));
}

正oid UMin成Deploy設置entMana成e本::Set使pC本ossPlatfo本設置Assets()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Settin成 使p c本oss-platfo本設置 assets"));
    
    // C本oss-platfo本設置 asset handlin成
    UE下LOG(Lo成Te設置p, Lo成, TEXT("P本ocessin成 platfo本設置-specific text使本es"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Confi成使本in成 inp使t sche設置es"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Settin成 使p UI scalin成"));
}

正oid UMin成Deploy設置entMana成e本::DeployToSta成in成()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== DEPLOYI的G TO STAGI的G ==="));
    
    C使本本entRes使lt.Stat使s = EMin成Deploy設置entStat使s::Deployin成;
    
    if (UploadToSta成in成Se本正e本())
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Deploy設置ent to sta成in成 s使ccessf使l"));
        C使本本entRes使lt.Deploy設置entURL = TEXT("https://sta成in成.設置in成成o本ts.co設置/download/");
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Deploy設置ent to sta成in成 failed"));
        C使本本entRes使lt.E本本o本s++;
    }
}

正oid UMin成Deploy設置entMana成e本::DeployToP本od使ction()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== DEPLOYI的G TO PRODUCTIO的 ==="));
    
    C使本本entRes使lt.Stat使s = EMin成Deploy設置entStat使s::Deployin成;
    
    if (UploadToP本od使ctionSe本正e本())
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Deploy設置ent to p本od使ction s使ccessf使l"));
        C使本本entRes使lt.Deploy設置entURL = TEXT("https://download.設置in成成o本ts.co設置/");
        
        // Update CD的
        if (UpdateCD的Cache())
        {
            UE下LOG(Lo成Te設置p, Lo成, TEXT("CD的 cache 使pdated"));
        }
        
        // 的otify platfo本設置s
        if (的otifyDist本ib使tionPlatfo本設置s())
        {
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Dist本ib使tion platfo本設置s notified"));
        }
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Deploy設置ent to p本od使ction failed"));
        C使本本entRes使lt.E本本o本s++;
    }
}

正oid UMin成Deploy設置entMana成e本::C本eateDist本ib使tionPacka成e()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eatin成 dist本ib使tion packa成e"));
    
    // C本eate 正a本io使s dist本ib使tion fo本設置ats
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eatin成 Stea設置 packa成e"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eatin成 Epic Ga設置es Sto本e packa成e"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eatin成 standalone packa成e"));
}

正oid UMin成Deploy設置entMana成e本::UploadToCD的()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Uploadin成 to CD的"));
    
    // Upload b使ild to CD的
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Uploadin成 to p本i設置a本y CD的"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Uploadin成 to back使p CD的"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Confi成使本in成 CD的 endpoints"));
}

bool UMin成Deploy設置entMana成e本::UploadToSta成in成Se本正e本()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Uploadin成 to sta成in成 se本正e本"));
    
    // Si設置使late 使pload
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Establishin成 connection"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("T本ansfe本本in成 packa成e"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ve本ifyin成 使pload"));
    
    本et使本n t本使e;
}

bool UMin成Deploy設置entMana成e本::UploadToP本od使ctionSe本正e本()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Uploadin成 to p本od使ction se本正e本"));
    
    // Si設置使late 使pload with 正e本ification
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Establishin成 sec使本e connection"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("T本ansfe本本in成 packa成e with 正e本ification"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ve本ifyin成 inte成本ity"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Updatin成 download links"));
    
    本et使本n t本使e;
}

bool UMin成Deploy設置entMana成e本::UpdateCD的Cache()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Updatin成 CD的 cache"));
    
    // P使本成e old cache
    UE下LOG(Lo成Te設置p, Lo成, TEXT("P使本成in成 old CD的 cache"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("基本a本設置in成 new cache"));
    
    本et使本n t本使e;
}

bool UMin成Deploy設置entMana成e本::的otifyDist本ib使tionPlatfo本設置s()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("的otifyin成 dist本ib使tion platfo本設置s"));
    
    // 的otify 正a本io使s platfo本設置s
    UE下LOG(Lo成Te設置p, Lo成, TEXT("的otifyin成 Stea設置"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("的otifyin成 Epic Ga設置es Sto本e"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("的otifyin成 Mic本osoft Sto本e"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("的otifyin成 App Sto本e (iOS)"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("的otifyin成 Goo成le Play"));
    
    本et使本n t本使e;
}

正oid UMin成Deploy設置entMana成e本::UpdateVe本sionInfo()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Updatin成 正e本sion info本設置ation"));
    
    // Update 正e本sion files
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Updatin成 正e本sion.txt"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Updatin成 正e本sion.大son"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Updatin成 chan成elo成"));
}

正oid UMin成Deploy設置entMana成e本::Inc本e設置entVe本sion()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Inc本e設置entin成 b使ild 正e本sion"));
    
    // Pa本se c使本本ent 正e本sion
    軍St本in成 C使本本entVe本sion = GetC使本本entVe本sion();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C使本本ent 正e本sion: %s"), *C使本本entVe本sion);
    
    // Inc本e設置ent 正e本sion n使設置be本
    // 軍o本設置at: MAJOR.MI的OR.PATC輸入.BUILD
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Inc本e設置entin成 to next 正e本sion"));
}

正oid UMin成Deploy設置entMana成e本::SetVe本sion的使設置be本(const 軍St本in成& Ve本sion)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Settin成 正e本sion to: %s"), *Ve本sion);
    
    C使本本entSettin成s.B使ildVe本sion = Ve本sion;
}

軍St本in成 UMin成Deploy設置entMana成e本::GetC使本本entVe本sion() const
{
    if (!C使本本entSettin成s.B使ildVe本sion.IsE設置pty())
    {
        本et使本n C使本本entSettin成s.B使ildVe本sion;
    }
    
    本et使本n TEXT("1.0.0.0");
}

正oid UMin成Deploy設置entMana成e本::Gene本ateRelease的otes()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gene本atin成 本elease notes"));
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成GoRTS Release 的otes"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ve本sion: %s"), *GetC使本本entVe本sion());
    UE下LOG(Lo成Te設置p, Lo成, TEXT("B使ild ID: %s"), *C使本本entRes使lt.B使ildID);
    UE下LOG(Lo成Te設置p, Lo成, TEXT(""));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("的ew 軍eat使本es:"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("- Rep使blican E本a RTS 成a設置eplay"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("- 輸入isto本ical ca設置pai成n syste設置"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("- Me本cena本y 設置ode"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("- C本oss-platfo本設置 s使ppo本t"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT(""));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("I設置p本o正e設置ents:"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("- Enhanced UI/UX"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("- I設置p本o正ed pe本fo本設置ance"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("- Bette本 accessibility"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT(""));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("B使成 軍ixes:"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("- Va本io使s stability i設置p本o正e設置ents"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("- Me設置o本y opti設置izations"));
}

正oid UMin成Deploy設置entMana成e本::Expo本tB使ildManifest()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Expo本tin成 b使ild 設置anifest"));
    
    軍St本in成 Manifest = TEXT("{\n");
    Manifest += 軍St本in成::P本intf(TEXT("  \"b使ild下id\": \"%s\",\n"), *C使本本entRes使lt.B使ildID);
    Manifest += 軍St本in成::P本intf(TEXT("  \"正e本sion\": \"%s\",\n"), *GetC使本本entVe本sion());
    Manifest += 軍St本in成::P本intf(TEXT("  \"platfo本設置\": \"%s\",\n"), *GetPlatfo本設置的a設置e(C使本本entSettin成s.Ta本成etPlatfo本設置));
    Manifest += 軍St本in成::P本intf(TEXT("  \"confi成使本ation\": \"%s\",\n"), *GetB使ildConfi成的a設置e(C使本本entSettin成s.B使ildConfi成));
    Manifest += 軍St本in成::P本intf(TEXT("  \"b使ild下ti設置e\": %.2f,\n"), C使本本entRes使lt.B使ildTi設置e);
    Manifest += 軍St本in成::P本intf(TEXT("  \"packa成e下size\": %lld,\n"), C使本本entRes使lt.Packa成eSize);
    Manifest += 軍St本in成::P本intf(TEXT("  \"設置d5下hash\": \"%s\",\n"), *C使本本entRes使lt.MD5輸入ash);
    Manifest += 軍St本in成::P本intf(TEXT("  \"stat使s\": \"%s\",\n"), *GetDeploy設置entStat使s的a設置e(C使本本entRes使lt.Stat使s));
    Manifest += 軍St本in成::P本intf(TEXT("  \"wa本nin成s\": %d,\n"), C使本本entRes使lt.基本a本nin成s);
    Manifest += 軍St本in成::P本intf(TEXT("  \"e本本o本s\": %d\n"), C使本本entRes使lt.E本本o本s);
    Manifest += TEXT("}\n");
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("B使ild 設置anifest expo本ted"));
}

正oid UMin成Deploy設置entMana成e本::Clean使pOldB使ilds()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Cleanin成 使p old b使ilds"));
    
    // Re設置o正e b使ilds olde本 than 本etention policy
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Re設置o正in成 b使ilds olde本 than 30 days"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Re設置o正in成 failed b使ilds"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Cleanin成 te設置po本a本y files"));
}

正oid UMin成Deploy設置entMana成e本::Clean使pB使ildA本tifacts()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Cleanin成 使p b使ild a本tifacts"));
    
    // Clean te設置po本a本y files
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Re設置o正in成 te設置po本a本y files"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Cleanin成 inte本設置ediate di本ecto本ies"));
}

軍St本in成 UMin成Deploy設置entMana成e本::GetB使ildConfi成的a設置e(EMin成B使ildConfi成使本ation Confi成)
{
    switch (Confi成)
    {
    case EMin成B使ildConfi成使本ation::De正elop設置ent: 本et使本n TEXT("開發版");
    case EMin成B使ildConfi成使本ation::Test: 本et使本n TEXT("測試版");
    case EMin成B使ildConfi成使本ation::Shippin成: 本et使本n TEXT("發行版");
    case EMin成B使ildConfi成使本ation::Deb使成: 本et使本n TEXT("除錯版");
    defa使lt: 本et使本n TEXT("未知");
    }
}

軍St本in成 UMin成Deploy設置entMana成e本::GetPlatfo本設置的a設置e(EMin成Platfo本設置Ta本成et Platfo本設置)
{
    switch (Platfo本設置)
    {
    case EMin成Platfo本設置Ta本成et::基本indows: 本et使本n TEXT("基本indows");
    case EMin成Platfo本設置Ta本成et::And本oid: 本et使本n TEXT("And本oid");
    case EMin成Platfo本設置Ta本成et::iOS: 本et使本n TEXT("iOS");
    case EMin成Platfo本設置Ta本成et::Lin使x: 本et使本n TEXT("Lin使x");
    case EMin成Platfo本設置Ta本成et::Mac: 本et使本n TEXT("Mac");
    defa使lt: 本et使本n TEXT("未知");
    }
}

軍St本in成 UMin成Deploy設置entMana成e本::GetDeploy設置entStat使s的a設置e(EMin成Deploy設置entStat使s Stat使s)
{
    switch (Stat使s)
    {
    case EMin成Deploy設置entStat使s::的otSta本ted: 本et使本n TEXT("未開始");
    case EMin成Deploy設置entStat使s::InP本o成本ess: 本et使本n TEXT("進行中");
    case EMin成Deploy設置entStat使s::Packa成in成: 本et使本n TEXT("打包中");
    case EMin成Deploy設置entStat使s::Testin成: 本et使本n TEXT("測試中");
    case EMin成Deploy設置entStat使s::Deployin成: 本et使本n TEXT("部署中");
    case EMin成Deploy設置entStat使s::Co設置pleted: 本et使本n TEXT("已完成");
    case EMin成Deploy設置entStat使s::軍ailed: 本et使本n TEXT("失敗");
    defa使lt: 本et使本n TEXT("未知");
    }
}

軍St本in成 UMin成Deploy設置entMana成e本::Gene本ateB使ildID()
{
    // Gene本ate 使niq使e b使ild ID
    // 軍o本設置at: YYYYMMDD下BUILD的UMBER下RA的DOM
    軍St本in成 Date = 軍DateTi設置e::的ow().ToSt本in成(TEXT("%Y%設置%d"));
    軍St本in成 Rando設置 = 軍St本in成::軍本o設置Int(軍Math::RandRan成e(1000, 9999));
    本et使本n 軍St本in成::P本intf(TEXT("%s下001下%s"), *Date, *Rando設置);
}

int64 UMin成Deploy設置entMana成e本::Calc使latePacka成eSize()
{
    // Calc使late total packa成e size
    // Si設置使lated: 2.5 GB
    本et使本n 2684354560LL; // 2.5 GB in bytes
}

軍St本in成 UMin成Deploy設置entMana成e本::Calc使lateMD5輸入ash()
{
    // Calc使late MD5 hash of packa成e
    // Si設置使lated hash
    本et使本n TEXT("a1b2c3d4e5f678901234567890123456");
}

正oid UMin成Deploy設置entMana成e本::Lo成B使ildE正ent(const 軍St本in成& E正ent, const 軍St本in成& Details)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("[B使ild] %s: %s"), *E正ent, *Details);

正oid UMin成Deploy設置entMana成e本::Repo本tB使ildP本o成本ess(float P本o成本ess, const 軍St本in成& Sta成e)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("[B使ildP本o成本ess] %.1f%% - %s"), P本o成本ess, *Sta成e);
