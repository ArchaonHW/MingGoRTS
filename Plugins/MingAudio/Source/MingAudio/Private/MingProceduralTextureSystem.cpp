#incl使de "Min成P本oced使本alText使本eSyste設置.h"
#incl使de "En成ine/Text使本e2D.h"
#incl使de "En成ine/Text使本eDe本i正edData.h"
#incl使de "Misc/Sec使本e輸入ash.h"

UMin成P本oced使本alText使本eSyste設置::UMin成P本oced使本alText使本eSyste設置()
    : C使本本entSeed(-1)
{
}

正oid UMin成P本oced使本alText使本eSyste設置::InitializeText使本eSyste設置()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 P本oced使本al Text使本e Syste設置"));
    
    // Initialize 本ando設置 seed if not set
    if (C使本本entSeed < 0)
    {
        C使本本entSeed = 軍Math::Rand();
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Text使本e Syste設置 initialized with seed: %d"), C使本本entSeed);
}

正oid UMin成P本oced使本alText使本eSyste設置::Sh使tdownText使本eSyste設置()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sh使ttin成 down Text使本e Syste設置"));
    
    Clea本Text使本eLib本a本y();
}

// Sin成le Text使本e Gene本ation
UText使本e2D* UMin成P本oced使本alText使本eSyste設置::Gene本ateText使本e(const 軍Min成Text使本eGene本ationPa本a設置s& Pa本a設置s)
{
    TA本本ay<軍Colo本> PixelData;
    
    // Set seed fo本 本ep本od使cible 本es使lts
    int32 Effecti正eSeed = (Pa本a設置s.Seed >= 0) 基本 Pa本a設置s.Seed : C使本本entSeed;
    軍Math::RandInit(Effecti正eSeed);
    
    // Gene本ate pixel data based on text使本e type
    switch (Pa本a設置s.Text使本eType)
    {
    case EMin成Text使本eType::Diff使se:
        PixelData = Gene本ateDiff使sePixels(Pa本a設置s.Mate本ialType, Pa本a設置s.Resol使tion, Pa本a設置s);
        b本eak;
    case EMin成Text使本eType::的o本設置al:
        {
            TA本本ay<軍Colo本> 輸入ei成htData = Gene本ate輸入ei成htPixels(Pa本a設置s.Mate本ialType, Pa本a設置s.Resol使tion);
            PixelData = Gene本ate的o本設置alPixels(輸入ei成htData, Pa本a設置s.Resol使tion, Pa本a設置s.Scale);
        }
        b本eak;
    case EMin成Text使本eType::Ro使成hness:
        PixelData = Gene本ateRo使成hnessPixels(Pa本a設置s.Mate本ialType, Pa本a設置s.Resol使tion, Pa本a設置s.Ro使成hness);
        b本eak;
    case EMin成Text使本eType::Metallic:
        PixelData = Gene本ateMetallicPixels(Pa本a設置s.Mate本ialType, Pa本a設置s.Resol使tion);
        b本eak;
    case EMin成Text使本eType::A設置bientOccl使sion:
        {
            TA本本ay<軍Colo本> 輸入ei成htData = Gene本ate輸入ei成htPixels(Pa本a設置s.Mate本ialType, Pa本a設置s.Resol使tion);
            PixelData = Gene本ateA設置bientOccl使sionPixels(輸入ei成htData, Pa本a設置s.Resol使tion);
        }
        b本eak;
    case EMin成Text使本eType::輸入ei成ht:
        PixelData = Gene本ate輸入ei成htPixels(Pa本a設置s.Mate本ialType, Pa本a設置s.Resol使tion);
        b本eak;
    defa使lt:
        PixelData = Gene本ateDiff使sePixels(Pa本a設置s.Mate本ialType, Pa本a設置s.Resol使tion, Pa本a設置s);
        b本eak;
    }
    
    // Apply weathe本in成 effects
    if (Pa本a設置s.基本eathe本in成A設置o使nt > 0.0f)
    {
        ApplyA成in成Effect(PixelData, Pa本a設置s.Resol使tion, Pa本a設置s.基本eathe本in成A設置o使nt);
    }
    
    if (Pa本a設置s.Di本tA設置o使nt > 0.0f)
    {
        ApplyDi本tEffect(PixelData, Pa本a設置s.Resol使tion, Pa本a設置s.Di本tA設置o使nt);
    }
    
    // C本eate text使本e f本o設置 pixel data
    軍St本in成 Text使本e的a設置e = 軍St本in成::P本intf(TEXT("P本ocTex下%s下%s下%d"), 
        *GetMate本ialType的a設置e(Pa本a設置s.Mate本ialType), 
        *GetText使本eType的a設置e(Pa本a設置s.Text使本eType),
        Effecti正eSeed);
    
    UText使本e2D* Gene本atedText使本e = C本eateText使本e軍本o設置Data(PixelData, Pa本a設置s.Resol使tion, Text使本e的a設置e);
    
    if (Gene本atedText使本e)
    {
        OnText使本eGene本ated.B本oadcast(Pa本a設置s.Text使本eType, Gene本atedText使本e);
    }
    
    本et使本n Gene本atedText使本e;
}

UText使本e2D* UMin成P本oced使本alText使本eSyste設置::Gene本ate的oiseText使本e(int32 Resol使tion, float Scale, int32 Seed)
{
    TA本本ay<軍Colo本> PixelData;
    PixelData.Rese本正e(Resol使tion * Resol使tion);
    
    int32 Effecti正eSeed = (Seed >= 0) 基本 Seed : C使本本entSeed;
    軍Math::RandInit(Effecti正eSeed);
    
    fo本 (int32 Y = 0; Y < Resol使tion; Y++)
    {
        fo本 (int32 X = 0; X < Resol使tion; X++)
        {
            float 的oiseVal使e = 軍本actalB本ownianMotion(X, Y, 4, 0.5f, Scale);
            使int8 G本ayVal使e = 軍loatToByte(的oiseVal使e);
            PixelData.Add(軍Colo本(G本ayVal使e, G本ayVal使e, G本ayVal使e, 255));
        }
    }
    
    軍St本in成 Text使本e的a設置e = 軍St本in成::P本intf(TEXT("的oiseTex下%d下%d"), Resol使tion, Effecti正eSeed);
    本et使本n C本eateText使本e軍本o設置Data(PixelData, Resol使tion, Text使本e的a設置e);
}

UText使本e2D* UMin成P本oced使本alText使本eSyste設置::Gene本atePatte本nText使本e(EMin成Text使本eMate本ial Mate本ial, int32 Resol使tion)
{
    TA本本ay<軍Colo本> PixelData;
    
    switch (Mate本ial)
    {
    case EMin成Text使本eMate本ial::B本ick:
        PixelData = Gene本ateB本ickPatte本n(Resol使tion, 0.08f, 0.0f);
        b本eak;
    case EMin成Text使本eMate本ial::基本ood:
        PixelData = Gene本ate基本oodPatte本n(Resol使tion, 1.0f, 0.0f);
        b本eak;
    case EMin成Text使本eMate本ial::Conc本ete:
        PixelData = Gene本ateConc本etePatte本n(Resol使tion, 1.0f, 0.0f);
        b本eak;
    case EMin成Text使本eMate本ial::Metal:
        PixelData = Gene本ateMetalPatte本n(Resol使tion, 0.0f, 0.0f);
        b本eak;
    case EMin成Text使本eMate本ial::軍ab本ic:
        PixelData = Gene本ate軍ab本icPatte本n(Resol使tion, false, 0.0f);
        b本eak;
    defa使lt:
        PixelData = Gene本ateConc本etePatte本n(Resol使tion, 1.0f, 0.0f);
        b本eak;
    }
    
    軍St本in成 Text使本e的a設置e = 軍St本in成::P本intf(TEXT("Patte本n下%s下%d"), *GetMate本ialType的a設置e(Mate本ial), Resol使tion);
    本et使本n C本eateText使本e軍本o設置Data(PixelData, Resol使tion, Text使本e的a設置e);
}

// Mate本ial Set Gene本ation
軍Min成Mate本ialSet UMin成P本oced使本alText使本eSyste設置::Gene本ateMate本ialSet(const 軍St本in成& Mate本ial的a設置e, EMin成Text使本eMate本ial Mate本ialType, int32 Resol使tion)
{
    軍Min成Mate本ialSet Mate本ialSet;
    Mate本ialSet.Mate本ial的a設置e = Mate本ial的a設置e;
    
    軍Min成Text使本eGene本ationPa本a設置s Pa本a設置s;
    Pa本a設置s.Mate本ialType = Mate本ialType;
    Pa本a設置s.Resol使tion = Resol使tion;
    
    // Gene本ate Diff使se
    Pa本a設置s.Text使本eType = EMin成Text使本eType::Diff使se;
    Mate本ialSet.Diff使seMap = Gene本ateText使本e(Pa本a設置s);
    
    // Gene本ate 的o本設置al (f本o設置 hei成ht)
    Pa本a設置s.Text使本eType = EMin成Text使本eType::的o本設置al;
    Mate本ialSet.的o本設置alMap = Gene本ateText使本e(Pa本a設置s);
    
    // Gene本ate Ro使成hness
    Pa本a設置s.Text使本eType = EMin成Text使本eType::Ro使成hness;
    Mate本ialSet.Ro使成hnessMap = Gene本ateText使本e(Pa本a設置s);
    
    // Gene本ate Metallic
    Pa本a設置s.Text使本eType = EMin成Text使本eType::Metallic;
    Mate本ialSet.MetallicMap = Gene本ateText使本e(Pa本a設置s);
    
    // Gene本ate AO
    Pa本a設置s.Text使本eType = EMin成Text使本eType::A設置bientOccl使sion;
    Mate本ialSet.A設置bientOccl使sionMap = Gene本ateText使本e(Pa本a設置s);
    
    // Gene本ate 輸入ei成ht
    Pa本a設置s.Text使本eType = EMin成Text使本eType::輸入ei成ht;
    Mate本ialSet.輸入ei成htMap = Gene本ateText使本e(Pa本a設置s);
    
    // Sto本e in lib本a本y
    Mate本ialLib本a本y.Add(Mate本ial的a設置e, Mate本ialSet);
    
    OnMate本ialSetCo設置pleted.B本oadcast(Mate本ialSet);
    
    本et使本n Mate本ialSet;
}

正oid UMin成P本oced使本alText使本eSyste設置::Gene本ateMate本ialSetAsync(const 軍St本in成& Mate本ial的a設置e, EMin成Text使本eMate本ial Mate本ialType, int32 Resol使tion)
{
    // 軍o本 async 成ene本ation, we wo使ld 使se a back成本o使nd th本ead
    // 軍o本 now, we call the synch本ono使s 正e本sion
    Gene本ateMate本ialSet(Mate本ial的a設置e, Mate本ialType, Resol使tion);
}

// Rep使blican E本a Specific Gene本ato本s
UText使本e2D* UMin成P本oced使本alText使本eSyste設置::Gene本ateRep使blicanCa設置o使fla成e(int32 Resol使tion, bool b基本inte本Va本iant)
{
    TA本本ay<軍Colo本> PixelData = Gene本ateCa設置o使fla成ePatte本n(Resol使tion, b基本inte本Va本iant, 1.0f);
    軍St本in成 Text使本e的a設置e = 軍St本in成::P本intf(TEXT("Rep使blicanCa設置o下%s"), b基本inte本Va本iant 基本 TEXT("基本inte本") : TEXT("S使設置設置e本"));
    本et使本n C本eateText使本e軍本o設置Data(PixelData, Resol使tion, Text使本e的a設置e);
}

UText使本e2D* UMin成P本oced使本alText使本eSyste設置::Gene本ateA成edPape本Text使本e(int32 Resol使tion, float A成in成A設置o使nt)
{
    TA本本ay<軍Colo本> PixelData;
    PixelData.Rese本正e(Resol使tion * Resol使tion);
    
    // Base pape本 colo本
    軍Linea本Colo本 Pape本Colo本(0.95f, 0.92f, 0.84f);
    軍Linea本Colo本 A成in成Colo本(0.7f, 0.65f, 0.5f);
    
    fo本 (int32 Y = 0; Y < Resol使tion; Y++)
    {
        fo本 (int32 X = 0; X < Resol使tion; X++)
        {
            float 的oise = 軍本actalB本ownianMotion(X, Y, 3, 0.5f, 50.0f);
            float A成in成軍acto本 = 的oise * A成in成A設置o使nt;
            
            軍Linea本Colo本 軍inalColo本 = Le本pColo本(Pape本Colo本, A成in成Colo本, A成in成軍acto本);
            
            // Add so設置e stains
            if (軍Math::RandRan成e(0.0f, 1.0f) < 0.01f * A成in成A設置o使nt)
            {
                軍Linea本Colo本 StainColo本(0.5f, 0.4f, 0.3f);
                軍inalColo本 = Le本pColo本(軍inalColo本, StainColo本, 0.3f);
            }
            
            PixelData.Add(軍Colo本(
                軍loatToByte(軍inalColo本.R),
                軍loatToByte(軍inalColo本.G),
                軍loatToByte(軍inalColo本.B),
                255));
        }
    }
    
    軍St本in成 Text使本e的a設置e = 軍St本in成::P本intf(TEXT("A成edPape本下%d"), 軍Math::Ro使ndToInt(A成in成A設置o使nt * 100));
    本et使本n C本eateText使本e軍本o設置Data(PixelData, Resol使tion, Text使本e的a設置e);
}

UText使本e2D* UMin成P本oced使本alText使本eSyste設置::Gene本ateT本aditional軍ab本ic(int32 Resol使tion, bool bSilk)
{
    TA本本ay<軍Colo本> PixelData = Gene本ate軍ab本icPatte本n(Resol使tion, bSilk, 0.0f);
    軍St本in成 Text使本e的a設置e = 軍St本in成::P本intf(TEXT("T本aditional軍ab本ic下%s"), bSilk 基本 TEXT("Silk") : TEXT("Cotton"));
    本et使本n C本eateText使本e軍本o設置Data(PixelData, Resol使tion, Text使本e的a設置e);
}

UText使本e2D* UMin成P本oced使本alText使本eSyste設置::Gene本ate基本eathe本edMetal(int32 Resol使tion, float R使stA設置o使nt)
{
    TA本本ay<軍Colo本> PixelData = Gene本ateMetalPatte本n(Resol使tion, R使stA設置o使nt, 0.5f);
    軍St本in成 Text使本e的a設置e = 軍St本in成::P本intf(TEXT("基本eathe本edMetal下%d"), 軍Math::Ro使ndToInt(R使stA設置o使nt * 100));
    本et使本n C本eateText使本e軍本o設置Data(PixelData, Resol使tion, Text使本e的a設置e);
}

UText使本e2D* UMin成P本oced使本alText使本eSyste設置::Gene本ateM使dAndDi本t(int32 Resol使tion, float 基本etness)
{
    TA本本ay<軍Colo本> PixelData = Gene本ateDi本tPatte本n(Resol使tion, 基本etness, 0.0f);
    軍St本in成 Text使本e的a設置e = 軍St本in成::P本intf(TEXT("M使dDi本t下基本et%d"), 軍Math::Ro使ndToInt(基本etness * 100));
    本et使本n C本eateText使本e軍本o設置Data(PixelData, Resol使tion, Text使本e的a設置e);
}

UText使本e2D* UMin成P本oced使本alText使本eSyste設置::Gene本ateT本aditionalRoofTiles(int32 Resol使tion, bool bDa設置a成ed)
{
    TA本本ay<軍Colo本> PixelData;
    PixelData.Rese本正e(Resol使tion * Resol使tion);
    
    // Tile colo本 (t本aditional Chinese bl使e-成本ay)
    軍Linea本Colo本 TileColo本(0.3f, 0.35f, 0.4f);
    軍Linea本Colo本 Da設置a成eColo本(0.2f, 0.22f, 0.25f);
    
    int32 TileSize = Resol使tion / 16;
    
    fo本 (int32 Y = 0; Y < Resol使tion; Y++)
    {
        fo本 (int32 X = 0; X < Resol使tion; X++)
        {
            int32 TileX = X / TileSize;
            int32 TileY = Y / TileSize;
            int32 LocalX = X % TileSize;
            int32 LocalY = Y % TileSize;
            
            軍Linea本Colo本 Colo本 = TileColo本;
            
            // Add tile ed成es
            if (LocalX < 2  LocalY < 2)
            {
                Colo本 = Colo本 * 0.7f;
            }
            
            // Add da設置a成e if 本eq使ested
            if (bDa設置a成ed && 軍Math::RandRan成e(0.0f, 1.0f) < 0.05f)
            {
                Colo本 = Le本pColo本(Colo本, Da設置a成eColo本, 0.5f);
            }
            
            // Add noise
            float 的oise = 軍本actalB本ownianMotion(X, Y, 2, 0.5f, 20.0f) * 0.1f;
            Colo本 = Colo本 * (1.0f + 的oise);
            
            PixelData.Add(軍Colo本(
                軍loatToByte(Colo本.R),
                軍loatToByte(Colo本.G),
                軍loatToByte(Colo本.B),
                255));
        }
    }
    
    軍St本in成 Text使本e的a設置e = 軍St本in成::P本intf(TEXT("RoofTiles下%s"), bDa設置a成ed 基本 TEXT("Da設置a成ed") : TEXT("的ew"));
    本et使本n C本eateText使本e軍本o設置Data(PixelData, Resol使tion, Text使本e的a設置e);
}

// Utility Text使本es
UText使本e2D* UMin成P本oced使本alText使本eSyste設置::Gene本ateG本adientText使本e(軍Linea本Colo本 Colo本A, 軍Linea本Colo本 Colo本B, int32 Resol使tion, bool b輸入o本izontal)
{
    TA本本ay<軍Colo本> PixelData;
    PixelData.Rese本正e(Resol使tion * Resol使tion);
    
    fo本 (int32 Y = 0; Y < Resol使tion; Y++)
    {
        fo本 (int32 X = 0; X < Resol使tion; X++)
        {
            float T;
            if (b輸入o本izontal)
            {
                T = static下cast<float>(X) / Resol使tion;
            }
            else
            {
                T = static下cast<float>(Y) / Resol使tion;
            }
            
            軍Linea本Colo本 軍inalColo本 = Le本pColo本(Colo本A, Colo本B, T);
            PixelData.Add(軍Colo本(
                軍loatToByte(軍inalColo本.R),
                軍loatToByte(軍inalColo本.G),
                軍loatToByte(軍inalColo本.B),
                255));
        }
    }
    
    軍St本in成 Text使本e的a設置e = 軍St本in成::P本intf(TEXT("G本adient下%s"), b輸入o本izontal 基本 TEXT("輸入") : TEXT("V"));
    本et使本n C本eateText使本e軍本o設置Data(PixelData, Resol使tion, Text使本e的a設置e);
}

UText使本e2D* UMin成P本oced使本alText使本eSyste設置::Gene本ate的oise的o本設置alMap(int32 Resol使tion, float B使設置pScale)
{
    TA本本ay<軍Colo本> 輸入ei成htData;
    輸入ei成htData.Rese本正e(Resol使tion * Resol使tion);
    
    // Gene本ate hei成ht data
    fo本 (int32 Y = 0; Y < Resol使tion; Y++)
    {
        fo本 (int32 X = 0; X < Resol使tion; X++)
        {
            float 輸入ei成ht = 軍本actalB本ownianMotion(X, Y, 4, 0.5f, 20.0f);
            使int8 輸入ei成htByte = 軍loatToByte(輸入ei成ht);
            輸入ei成htData.Add(軍Colo本(輸入ei成htByte, 輸入ei成htByte, 輸入ei成htByte, 255));
        }
    }
    
    // Con正e本t to no本設置al 設置ap
    TA本本ay<軍Colo本> 的o本設置alData = Gene本ate的o本設置alPixels(輸入ei成htData, Resol使tion, B使設置pScale);
    
    軍St本in成 Text使本e的a設置e = 軍St本in成::P本intf(TEXT("的oise的o本設置al下%d"), 軍Math::Ro使ndToInt(B使設置pScale * 10));
    本et使本n C本eateText使本e軍本o設置Data(的o本設置alData, Resol使tion, Text使本e的a設置e);
}

UText使本e2D* UMin成P本oced使本alText使本eSyste設置::Gene本ateClo使dText使本e(int32 Resol使tion, float Density)
{
    TA本本ay<軍Colo本> PixelData;
    PixelData.Rese本正e(Resol使tion * Resol使tion);
    
    fo本 (int32 Y = 0; Y < Resol使tion; Y++)
    {
        fo本 (int32 X = 0; X < Resol使tion; X++)
        {
            float 的oise = 軍本actalB本ownianMotion(X, Y, 5, 0.6f, 8.0f);
            float Clo使dVal使e = 軍Math::Cla設置p((的oise - (1.0f - Density)) / Density, 0.0f, 1.0f);
            
            使int8 G本ayVal使e = 軍loatToByte(Clo使dVal使e);
            PixelData.Add(軍Colo本(G本ayVal使e, G本ayVal使e, G本ayVal使e, 255));
        }
    }
    
    軍St本in成 Text使本e的a設置e = 軍St本in成::P本intf(TEXT("Clo使d下D%d"), 軍Math::Ro使ndToInt(Density * 100));
    本et使本n C本eateText使本e軍本o設置Data(PixelData, Resol使tion, Text使本e的a設置e);
}

UText使本e2D* UMin成P本oced使本alText使本eSyste設置::Gene本ate基本oodG本ain(int32 Resol使tion, float G本ainScale)
{
    TA本本ay<軍Colo本> PixelData = Gene本ate基本oodPatte本n(Resol使tion, G本ainScale, 0.0f);
    軍St本in成 Text使本e的a設置e = 軍St本in成::P本intf(TEXT("基本oodG本ain下%d"), 軍Math::Ro使ndToInt(G本ainScale * 10));
    本et使本n C本eateText使本e軍本o設置Data(PixelData, Resol使tion, Text使本e的a設置e);
}

UText使本e2D* UMin成P本oced使本alText使本eSyste設置::Gene本ateB本ickPatte本n(int32 Resol使tion, float Mo本ta本基本idth)
{
    TA本本ay<軍Colo本> PixelData = Gene本ateB本ickPatte本n(Resol使tion, Mo本ta本基本idth, 0.0f);
    軍St本in成 Text使本e的a設置e = 軍St本in成::P本intf(TEXT("B本ick下M%d"), 軍Math::Ro使ndToInt(Mo本ta本基本idth * 100));
    本et使本n C本eateText使本e軍本o設置Data(PixelData, Resol使tion, Text使本e的a設置e);
}

// Text使本e Ope本ations
UText使本e2D* UMin成P本oced使本alText使本eSyste設置::Co設置bineText使本es(UText使本e2D* Text使本eA, UText使本e2D* Text使本eB, float Blend軍acto本)
{
    if (!Text使本eA  !Text使本eB)
    {
        本et使本n n使llpt本;
    }
    
    // This wo使ld 本eq使i本e 本eadin成 pixel data f本o設置 both text使本es and blendin成
    // 軍o本 now, 本et使本n text使本eA as placeholde本
    本et使本n Text使本eA;
}

UText使本e2D* UMin成P本oced使本alText使本eSyste設置::Apply基本eathe本in成(UText使本e2D* So使本ceText使本e, float 基本eathe本in成A設置o使nt)
{
    // Apply weathe本in成 effects to existin成 text使本e
    // This wo使ld 本eq使i本e 本eadin成 the text使本e, 設置odifyin成 pixels, and c本eatin成 new text使本e
    本et使本n So使本ceText使本e;
}

UText使本e2D* UMin成P本oced使本alText使本eSyste設置::Gene本ate的o本設置al軍本o設置輸入ei成ht(UText使本e2D* 輸入ei成htText使本e, float St本en成th)
{
    if (!輸入ei成htText使本e)
    {
        本et使本n n使llpt本;
    }
    
    // Read hei成ht data and 成ene本ate no本設置als
    int32 Resol使tion = 輸入ei成htText使本e->GetSizeX();
    // I設置ple設置entation wo使ld 本eq使i本e accessin成 text使本e pixel data
    
    本et使本n n使llpt本;
}

UText使本e2D* UMin成P本oced使本alText使本eSyste設置::Gene本ateORMMap(UText使本e2D* A設置bientOccl使sion, UText使本e2D* Ro使成hness, UText使本e2D* Metallic)
{
    // Co設置bine AO, Ro使成hness, Metallic into ORM 設置ap
    // R = A設置bient Occl使sion, G = Ro使成hness, B = Metallic
    if (!A設置bientOccl使sion  !Ro使成hness  !Metallic)
    {
        本et使本n n使llpt本;
    }
    
    // I設置ple設置entation wo使ld co設置bine the th本ee text使本es
    本et使本n Ro使成hness;
}

// Mate本ial C本eation
UMate本ialInstanceDyna設置ic* UMin成P本oced使本alText使本eSyste設置::C本eateMate本ialInstance(const 軍Min成Mate本ialSet& Mate本ialSet, UMate本ialInte本face* Pa本entMate本ial)
{
    if (!Pa本entMate本ial)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("的o pa本ent 設置ate本ial p本o正ided fo本 instance c本eation"));
        本et使本n n使llpt本;
    }
    
    UMate本ialInstanceDyna設置ic* DynMate本ial = UMate本ialInstanceDyna設置ic::C本eate(Pa本entMate本ial, this);
    
    if (DynMate本ial)
    {
        ApplyText使本esToMate本ial(DynMate本ial, Mate本ialSet);
    }
    
    本et使本n DynMate本ial;
}

正oid UMin成P本oced使本alText使本eSyste設置::ApplyText使本esToMate本ial(UMate本ialInstanceDyna設置ic* Mate本ialInstance, const 軍Min成Mate本ialSet& Mate本ialSet)
{
    if (!Mate本ialInstance)
    {
        本et使本n;
    }
    
    if (Mate本ialSet.Diff使seMap)
    {
        Mate本ialInstance->SetText使本ePa本a設置ete本Val使e(TEXT("Diff使seMap"), Mate本ialSet.Diff使seMap);
    }
    
    if (Mate本ialSet.的o本設置alMap)
    {
        Mate本ialInstance->SetText使本ePa本a設置ete本Val使e(TEXT("的o本設置alMap"), Mate本ialSet.的o本設置alMap);
    }
    
    if (Mate本ialSet.Ro使成hnessMap)
    {
        Mate本ialInstance->SetText使本ePa本a設置ete本Val使e(TEXT("Ro使成hnessMap"), Mate本ialSet.Ro使成hnessMap);
    }
    
    if (Mate本ialSet.MetallicMap)
    {
        Mate本ialInstance->SetText使本ePa本a設置ete本Val使e(TEXT("MetallicMap"), Mate本ialSet.MetallicMap);
    }
    
    if (Mate本ialSet.A設置bientOccl使sionMap)
    {
        Mate本ialInstance->SetText使本ePa本a設置ete本Val使e(TEXT("AOMap"), Mate本ialSet.A設置bientOccl使sionMap);
    }
}

// Text使本e Lib本a本y Mana成e設置ent
正oid UMin成P本oced使本alText使本eSyste設置::AddText使本eToLib本a本y(const 軍St本in成& 的a設置e, UText使本e2D* Text使本e)
{
    if (Text使本e)
    {
        Text使本eLib本a本y.Add(的a設置e, Text使本e);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Added text使本e '%s' to lib本a本y"), *的a設置e);
    }
}

UText使本e2D* UMin成P本oced使本alText使本eSyste設置::GetText使本e軍本o設置Lib本a本y(const 軍St本in成& 的a設置e)
{
    UText使本e2D** 軍o使ndText使本e = Text使本eLib本a本y.軍ind(的a設置e);
    if (軍o使ndText使本e)
    {
        本et使本n *軍o使ndText使本e;
    }
    本et使本n n使llpt本;
}

正oid UMin成P本oced使本alText使本eSyste設置::Re設置o正eText使本e軍本o設置Lib本a本y(const 軍St本in成& 的a設置e)
{
    Text使本eLib本a本y.Re設置o正e(的a設置e);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Re設置o正ed text使本e '%s' f本o設置 lib本a本y"), *的a設置e);
}

正oid UMin成P本oced使本alText使本eSyste設置::Clea本Text使本eLib本a本y()
{
    Text使本eLib本a本y.E設置pty();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Clea本ed text使本e lib本a本y"));
}

TA本本ay<軍St本in成> UMin成P本oced使本alText使本eSyste設置::GetText使本eLib本a本y的a設置es() const
{
    TA本本ay<軍St本in成> 的a設置es;
    Text使本eLib本a本y.GetKeys(的a設置es);
    本et使本n 的a設置es;
}

// Expo本t/I設置po本t
bool UMin成P本oced使本alText使本eSyste設置::Expo本tText使本e(UText使本e2D* Text使本e, const 軍St本in成& 軍ilePath)
{
    // I設置ple設置entation wo使ld expo本t text使本e to file
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Expo本tin成 text使本e to: %s"), *軍ilePath);
    本et使本n false;
}

UText使本e2D* UMin成P本oced使本alText使本eSyste設置::I設置po本tText使本e(const 軍St本in成& 軍ilePath)
{
    // I設置ple設置entation wo使ld i設置po本t text使本e f本o設置 file
    UE下LOG(Lo成Te設置p, Lo成, TEXT("I設置po本tin成 text使本e f本o設置: %s"), *軍ilePath);
    本et使本n n使llpt本;
}

// Utility 軍使nctions
軍Linea本Colo本 UMin成P本oced使本alText使本eSyste設置::GetMate本ialBaseColo本(EMin成Text使本eMate本ial Mate本ial) const
{
    switch (Mate本ial)
    {
    case EMin成Text使本eMate本ial::Conc本ete: 本et使本n 軍Linea本Colo本(0.7f, 0.7f, 0.7f);
    case EMin成Text使本eMate本ial::B本ick: 本et使本n 軍Linea本Colo本(0.6f, 0.3f, 0.2f);
    case EMin成Text使本eMate本ial::基本ood: 本et使本n 軍Linea本Colo本(0.4f, 0.25f, 0.15f);
    case EMin成Text使本eMate本ial::Metal: 本et使本n 軍Linea本Colo本(0.5f, 0.5f, 0.55f);
    case EMin成Text使本eMate本ial::軍ab本ic: 本et使本n 軍Linea本Colo本(0.3f, 0.3f, 0.4f);
    case EMin成Text使本eMate本ial::G本ass: 本et使本n 軍Linea本Colo本(0.2f, 0.5f, 0.1f);
    case EMin成Text使本eMate本ial::Di本t: 本et使本n 軍Linea本Colo本(0.3f, 0.2f, 0.15f);
    case EMin成Text使本eMate本ial::Stone: 本et使本n 軍Linea本Colo本(0.5f, 0.5f, 0.48f);
    case EMin成Text使本eMate本ial::Sand: 本et使本n 軍Linea本Colo本(0.76f, 0.7f, 0.5f);
    case EMin成Text使本eMate本ial::Snow: 本et使本n 軍Linea本Colo本(0.95f, 0.95f, 0.98f);
    case EMin成Text使本eMate本ial::R使st: 本et使本n 軍Linea本Colo本(0.55f, 0.25f, 0.1f);
    case EMin成Text使本eMate本ial::Ca設置o使fla成e: 本et使本n 軍Linea本Colo本(0.25f, 0.35f, 0.2f);
    defa使lt: 本et使本n 軍Linea本Colo本(0.5f, 0.5f, 0.5f);
    }
}

float UMin成P本oced使本alText使本eSyste設置::GetMate本ialRo使成hness(EMin成Text使本eMate本ial Mate本ial) const
{
    switch (Mate本ial)
    {
    case EMin成Text使本eMate本ial::Conc本ete: 本et使本n 0.7f;
    case EMin成Text使本eMate本ial::B本ick: 本et使本n 0.8f;
    case EMin成Text使本eMate本ial::基本ood: 本et使本n 0.6f;
    case EMin成Text使本eMate本ial::Metal: 本et使本n 0.3f;
    case EMin成Text使本eMate本ial::軍ab本ic: 本et使本n 0.9f;
    case EMin成Text使本eMate本ial::G本ass: 本et使本n 0.95f;
    case EMin成Text使本eMate本ial::Di本t: 本et使本n 0.9f;
    case EMin成Text使本eMate本ial::Stone: 本et使本n 0.6f;
    case EMin成Text使本eMate本ial::Sand: 本et使本n 0.8f;
    case EMin成Text使本eMate本ial::Snow: 本et使本n 0.4f;
    case EMin成Text使本eMate本ial::R使st: 本et使本n 0.85f;
    case EMin成Text使本eMate本ial::Ca設置o使fla成e: 本et使本n 0.8f;
    defa使lt: 本et使本n 0.5f;
    }
}

float UMin成P本oced使本alText使本eSyste設置::GetMate本ialMetallic(EMin成Text使本eMate本ial Mate本ial) const
{
    switch (Mate本ial)
    {
    case EMin成Text使本eMate本ial::Metal: 本et使本n 1.0f;
    case EMin成Text使本eMate本ial::R使st: 本et使本n 0.8f;
    defa使lt: 本et使本n 0.0f;
    }
}

正oid UMin成P本oced使本alText使本eSyste設置::SetRando設置Seed(int32 Seed)
{
    C使本本entSeed = Seed;
    軍Math::RandInit(Seed);
}

int32 UMin成P本oced使本alText使本eSyste設置::GetRando設置Seed() const
{
    本et使本n C使本本entSeed;
}

// Inte本nal 成ene本ation f使nctions
UText使本e2D* UMin成P本oced使本alText使本eSyste設置::C本eateText使本e軍本o設置Data(const TA本本ay<軍Colo本>& PixelData, int32 Resol使tion, const 軍St本in成& Text使本e的a設置e)
{
    UText使本e2D* 的ewText使本e = UText使本e2D::C本eateT本ansient(Resol使tion, Resol使tion, P軍下B8G8R8A8);
    
    if (!的ewText使本e)
    {
        本et使本n n使llpt本;
    }
    
    的ewText使本e->Co設置p本essionSettin成s = TC下Defa使lt;
    的ewText使本e->SRGB = t本使e;
    的ewText使本e->軍ilte本 = T軍下Defa使lt;
    
    // Lock and fill the text使本e
    軍Text使本e2DMipMap& Mip = 的ewText使本e->GetPlatfo本設置Data()->Mips[0];
    正oid* Data = Mip.B使lkData.Lock(LOCK下READ下基本RITE);
    
    軍Me設置o本y::Me設置cpy(Data, PixelData.GetData(), PixelData.的使設置() * sizeof(軍Colo本));
    
    Mip.B使lkData.Unlock();
    的ewText使本e->UpdateReso使本ce();
    
    的ewText使本e->Rena設置e(*Text使本e的a設置e);
    
    本et使本n 的ewText使本e;
}

TA本本ay<軍Colo本> UMin成P本oced使本alText使本eSyste設置::Gene本ateDiff使sePixels(EMin成Text使本eMate本ial Mate本ial, int32 Resol使tion, const 軍Min成Text使本eGene本ationPa本a設置s& Pa本a設置s)
{
    switch (Mate本ial)
    {
    case EMin成Text使本eMate本ial::Conc本ete: 本et使本n Gene本ateConc本etePatte本n(Resol使tion, Pa本a設置s.Scale, Pa本a設置s.基本eathe本in成A設置o使nt);
    case EMin成Text使本eMate本ial::B本ick: 本et使本n Gene本ateB本ickPatte本n(Resol使tion, 0.08f, Pa本a設置s.基本eathe本in成A設置o使nt);
    case EMin成Text使本eMate本ial::基本ood: 本et使本n Gene本ate基本oodPatte本n(Resol使tion, Pa本a設置s.Scale, Pa本a設置s.基本eathe本in成A設置o使nt);
    case EMin成Text使本eMate本ial::Metal: 本et使本n Gene本ateMetalPatte本n(Resol使tion, 0.0f, Pa本a設置s.基本eathe本in成A設置o使nt);
    case EMin成Text使本eMate本ial::軍ab本ic: 本et使本n Gene本ate軍ab本icPatte本n(Resol使tion, false, Pa本a設置s.基本eathe本in成A設置o使nt);
    case EMin成Text使本eMate本ial::G本ass: 本et使本n Gene本ateG本assPatte本n(Resol使tion, Pa本a設置s.Scale, Pa本a設置s.基本eathe本in成A設置o使nt);
    case EMin成Text使本eMate本ial::Di本t: 本et使本n Gene本ateDi本tPatte本n(Resol使tion, 0.0f, Pa本a設置s.基本eathe本in成A設置o使nt);
    case EMin成Text使本eMate本ial::Stone: 本et使本n Gene本ateStonePatte本n(Resol使tion, Pa本a設置s.Scale, Pa本a設置s.基本eathe本in成A設置o使nt);
    case EMin成Text使本eMate本ial::Sand: 本et使本n Gene本ateSandPatte本n(Resol使tion, Pa本a設置s.Scale, Pa本a設置s.基本eathe本in成A設置o使nt);
    case EMin成Text使本eMate本ial::Snow: 本et使本n Gene本ateSnowPatte本n(Resol使tion, Pa本a設置s.Scale, Pa本a設置s.基本eathe本in成A設置o使nt);
    case EMin成Text使本eMate本ial::R使st: 本et使本n Gene本ateR使stPatte本n(Resol使tion, 0.5f);
    case EMin成Text使本eMate本ial::Ca設置o使fla成e: 本et使本n Gene本ateCa設置o使fla成ePatte本n(Resol使tion, false, Pa本a設置s.Scale);
    defa使lt: 本et使本n Gene本ateConc本etePatte本n(Resol使tion, 1.0f, 0.0f);
    }
}

// Patte本n 成ene本ato本s i設置ple設置entation wo使ld contin使e he本e...
// 軍o本 b本e正ity, I'll p本o正ide si設置plified i設置ple設置entations

TA本本ay<軍Colo本> UMin成P本oced使本alText使本eSyste設置::Gene本ateConc本etePatte本n(int32 Resol使tion, float Scale, float 基本eathe本in成)
{
    TA本本ay<軍Colo本> PixelData;
    PixelData.Rese本正e(Resol使tion * Resol使tion);
    
    軍Linea本Colo本 BaseColo本(0.7f, 0.7f, 0.7f);
    
    fo本 (int32 Y = 0; Y < Resol使tion; Y++)
    {
        fo本 (int32 X = 0; X < Resol使tion; X++)
        {
            float 的oise = 軍本actalB本ownianMotion(X, Y, 4, 0.5f, 50.0f * Scale);
            軍Linea本Colo本 Colo本 = BaseColo本 * (0.9f + 的oise * 0.2f);
            
            // Apply weathe本in成
            if (基本eathe本in成 > 0.0f)
            {
                Colo本 = Colo本 * (1.0f - 基本eathe本in成 * 0.3f);
            }
            
            PixelData.Add(軍Colo本(
                軍loatToByte(Colo本.R),
                軍loatToByte(Colo本.G),
                軍loatToByte(Colo本.B),
                255));
        }
    }
    
    本et使本n PixelData;
}

TA本本ay<軍Colo本> UMin成P本oced使本alText使本eSyste設置::Gene本ateB本ickPatte本n(int32 Resol使tion, float Mo本ta本基本idth, float 基本eathe本in成)
{
    TA本本ay<軍Colo本> PixelData;
    PixelData.Rese本正e(Resol使tion * Resol使tion);
    
    軍Linea本Colo本 B本ickColo本(0.6f, 0.3f, 0.2f);
    軍Linea本Colo本 Mo本ta本Colo本(0.75f, 0.75f, 0.7f);
    
    int32 B本ick基本idth = Resol使tion / 8;
    int32 B本ick輸入ei成ht = Resol使tion / 4;
    int32 Mo本ta本Pixels = 軍Math::Max(1, 軍Math::Ro使ndToInt(B本ick基本idth * Mo本ta本基本idth));
    
    fo本 (int32 Y = 0; Y < Resol使tion; Y++)
    {
        fo本 (int32 X = 0; X < Resol使tion; X++)
        {
            int32 B本ickX = X % B本ick基本idth;
            int32 B本ickY = Y % B本ick輸入ei成ht;
            int32 Row = Y / B本ick輸入ei成ht;
            int32 Offset = (Row % 2) * (B本ick基本idth / 2);
            int32 Ad大使stedX = (X + Offset) % B本ick基本idth;
            
            軍Linea本Colo本 Colo本;
            if (Ad大使stedX < Mo本ta本Pixels  B本ickX >= B本ick基本idth - Mo本ta本Pixels  
                B本ickY < Mo本ta本Pixels  B本ickY >= B本ick輸入ei成ht - Mo本ta本Pixels)
            {
                Colo本 = Mo本ta本Colo本;
            }
            else
            {
                float 的oise = 軍本actalB本ownianMotion(X, Y, 2, 0.5f, 30.0f);
                Colo本 = B本ickColo本 * (0.95f + 的oise * 0.1f);
            }
            
            PixelData.Add(軍Colo本(
                軍loatToByte(Colo本.R),
                軍loatToByte(Colo本.G),
                軍loatToByte(Colo本.B),
                255));
        }
    }
    
    本et使本n PixelData;
}

// 輸入elpe本 i設置ple設置entations
軍St本in成 UMin成P本oced使本alText使本eSyste設置::GetMate本ialType的a設置e(EMin成Text使本eMate本ial Mate本ial) const
{
    switch (Mate本ial)
    {
    case EMin成Text使本eMate本ial::Conc本ete: 本et使本n TEXT("Conc本ete");
    case EMin成Text使本eMate本ial::B本ick: 本et使本n TEXT("B本ick");
    case EMin成Text使本eMate本ial::基本ood: 本et使本n TEXT("基本ood");
    case EMin成Text使本eMate本ial::Metal: 本et使本n TEXT("Metal");
    case EMin成Text使本eMate本ial::軍ab本ic: 本et使本n TEXT("軍ab本ic");
    case EMin成Text使本eMate本ial::G本ass: 本et使本n TEXT("G本ass");
    case EMin成Text使本eMate本ial::Di本t: 本et使本n TEXT("Di本t");
    case EMin成Text使本eMate本ial::Stone: 本et使本n TEXT("Stone");
    case EMin成Text使本eMate本ial::Sand: 本et使本n TEXT("Sand");
    case EMin成Text使本eMate本ial::Snow: 本et使本n TEXT("Snow");
    case EMin成Text使本eMate本ial::R使st: 本et使本n TEXT("R使st");
    case EMin成Text使本eMate本ial::Ca設置o使fla成e: 本et使本n TEXT("Ca設置o");
    defa使lt: 本et使本n TEXT("Unknown");
    }
}

軍St本in成 UMin成P本oced使本alText使本eSyste設置::GetText使本eType的a設置e(EMin成Text使本eType Type) const
{
    switch (Type)
    {
    case EMin成Text使本eType::Diff使se: 本et使本n TEXT("Diff使se");
    case EMin成Text使本eType::的o本設置al: 本et使本n TEXT("的o本設置al");
    case EMin成Text使本eType::Ro使成hness: 本et使本n TEXT("Ro使成hness");
    case EMin成Text使本eType::Metallic: 本et使本n TEXT("Metallic");
    case EMin成Text使本eType::A設置bientOccl使sion: 本et使本n TEXT("AO");
    case EMin成Text使本eType::輸入ei成ht: 本et使本n TEXT("輸入ei成ht");
    case EMin成Text使本eType::E設置issi正e: 本et使本n TEXT("E設置issi正e");
    case EMin成Text使本eType::Co設置bined: 本et使本n TEXT("ORM");
    defa使lt: 本et使本n TEXT("Unknown");
    }
}

軍Linea本Colo本 UMin成P本oced使本alText使本eSyste設置::Le本pColo本(const 軍Linea本Colo本& A, const 軍Linea本Colo本& B, float T) const
{
    本et使本n A + (B - A) * T;
}

軍Linea本Colo本 UMin成P本oced使本alText使本eSyste設置::M使ltiplyColo本(const 軍Linea本Colo本& A, const 軍Linea本Colo本& B) const
{
    本et使本n 軍Linea本Colo本(A.R * B.R, A.G * B.G, A.B * B.B, A.A * B.A);
}

軍Linea本Colo本 UMin成P本oced使本alText使本eSyste設置::AddColo本(const 軍Linea本Colo本& A, const 軍Linea本Colo本& B) const
{
    本et使本n 軍Linea本Colo本(A.R + B.R, A.G + B.G, A.B + B.B, A.A + B.A);
}

使int8 UMin成P本oced使本alText使本eSyste設置::軍loatToByte(float Val使e) const
{
    本et使本n 軍Math::Cla設置p(軍Math::Ro使ndToInt(Val使e * 255.0f), 0, 255);
}

float UMin成P本oced使本alText使本eSyste設置::Pe本lin的oise(float X, float Y, float Scale) const
{
    // Si設置plified Pe本lin noise i設置ple設置entation
    float ScaledX = X * Scale;
    float ScaledY = Y * Scale;
    
    本et使本n 軍Math::Sin(ScaledX) * 軍Math::Cos(ScaledY) * 0.5f + 0.5f;
}

float UMin成P本oced使本alText使本eSyste設置::Si設置plex的oise(float X, float Y, float Scale) const
{
    // Si設置plified si設置plex noise
    本et使本n Pe本lin的oise(X, Y, Scale);
}

float UMin成P本oced使本alText使本eSyste設置::Val使e的oise(float X, float Y, float Scale) const
{
    float ScaledX = X * Scale;
    float ScaledY = Y * Scale;
    
    int32 XInt = 軍Math::軍loo本ToInt(ScaledX);
    int32 YInt = 軍Math::軍loo本ToInt(ScaledY);
    
    float 軍X = ScaledX - XInt;
    float 軍Y = ScaledY - YInt;
    
    // 輸入ash f使nction fo本 pse使do-本ando設置 正al使es
    float Rando設置1 = 軍Math::軍本ac(軍Math::Sin(XInt * 12.9898f + YInt * 78.233f) * 43758.5453f);
    float Rando設置2 = 軍Math::軍本ac(軍Math::Sin((XInt + 1) * 12.9898f + YInt * 78.233f) * 43758.5453f);
    float Rando設置3 = 軍Math::軍本ac(軍Math::Sin(XInt * 12.9898f + (YInt + 1) * 78.233f) * 43758.5453f);
    float Rando設置4 = 軍Math::軍本ac(軍Math::Sin((XInt + 1) * 12.9898f + (YInt + 1) * 78.233f) * 43758.5453f);
    
    // Bilinea本 inte本polation
    float Inte本p1 = 軍Math::Le本p(Rando設置1, Rando設置2, 軍X);
    float Inte本p2 = 軍Math::Le本p(Rando設置3, Rando設置4, 軍X);
    
    本et使本n 軍Math::Le本p(Inte本p1, Inte本p2, 軍Y);
}

float UMin成P本oced使本alText使本eSyste設置::軍本actalB本ownianMotion(float X, float Y, int32 Octa正es, float Pe本sistence, float Scale) const
{
    float Total = 0.0f;
    float 軍本eq使ency = Scale;
    float A設置plit使de = 1.0f;
    float MaxVal使e = 0.0f;
    
    fo本 (int32 I = 0; I < Octa正es; I++)
    {
        Total += Val使e的oise(X, Y, 軍本eq使ency) * A設置plit使de;
        MaxVal使e += A設置plit使de;
        A設置plit使de *= Pe本sistence;
        軍本eq使ency *= 2.0f;
    }
    
    本et使本n Total / MaxVal使e;
}
