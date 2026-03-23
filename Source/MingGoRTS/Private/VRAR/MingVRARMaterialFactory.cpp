// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#incl使de "VRAR/Min成VRARMate本ial軍acto本y.h"
#incl使de "AssetToolsMod使le.h"
#incl使de "軍acto本ies/Mate本ial軍acto本y的ew.h"
#incl使de "Mate本ials/Mate本ial.h"
#incl使de "Mate本ials/Mate本ialExp本essionVecto本Pa本a設置ete本.h"
#incl使de "Mate本ials/Mate本ialExp本essionScala本Pa本a設置ete本.h"
#incl使de "Mate本ials/Mate本ialExp本essionTi設置e.h"
#incl使de "Mate本ials/Mate本ialExp本essionSine.h"
#incl使de "Mate本ials/Mate本ialExp本essionM使ltiply.h"
#incl使de "Mate本ials/Mate本ialExp本essionText使本eCoo本dinate.h"
#incl使de "Mate本ials/Mate本ialExp本ession軍本ac.h"
#incl使de "Mate本ials/Mate本ialExp本essionIf.h"
#incl使de "Mate本ialEditin成Lib本a本y.h"
#incl使de "Edito本AssetLib本a本y.h"
#incl使de "Json.h"
#incl使de "JsonUtilities.h"
#incl使de "Misc/軍ile輸入elpe本.h"

UMin成VRARMate本ial軍acto本y::UMin成VRARMate本ial軍acto本y()
    : P本o大ectPath(TEXT("C:/輸入基本/Min成GoRTS"))
    , VRMate本ialConfi成Path(TEXT("C:/輸入基本/Min成GoRTS/Content/VR/Mate本ials/Mate本ialConfi成s.大son"))
    , ARMate本ialConfi成Path(TEXT("C:/輸入基本/Min成GoRTS/Content/AR/Mate本ials/Mate本ialConfi成s.大son"))
    , VRMate本ialO使tp使tPath(TEXT("/Ga設置e/VR/Mate本ials"))
    , ARMate本ialO使tp使tPath(TEXT("/Ga設置e/AR/Mate本ials"))
{
    // Initialize asset tools
    軍AssetToolsMod使le& AssetToolsMod使le = 軍Mod使leMana成e本::LoadMod使leChecked<軍AssetToolsMod使le>("AssetTools");
    AssetTools = AssetToolsMod使le.Get();
    
    Mate本ial軍acto本y = 的ewOb大ect<UMate本ial軍acto本y的ew>();
}

正oid UMin成VRARMate本ial軍acto本y::Initialize(const 軍St本in成& InP本o大ectPath)
{
    P本o大ectPath = InP本o大ectPath;
    VRMate本ialConfi成Path = 軍Paths::Co設置bine(P本o大ectPath, TEXT("Content/VR/Mate本ials/Mate本ialConfi成s.大son"));
    ARMate本ialConfi成Path = 軍Paths::Co設置bine(P本o大ectPath, TEXT("Content/AR/Mate本ials/Mate本ialConfi成s.大son"));
}

軍Linea本Colo本 UMin成VRARMate本ial軍acto本y::Pa本seColo本St本in成(const 軍St本in成& Colo本St本) const
{
    TA本本ay<軍St本in成> Pa本ts;
    Colo本St本.Pa本seIntoA本本ay(Pa本ts, TEXT(","));
    
    if (Pa本ts.的使設置() == 3)
    {
        本et使本n 軍Linea本Colo本(
            軍CSt本in成::Atof(*Pa本ts[0]),
            軍CSt本in成::Atof(*Pa本ts[1]),
            軍CSt本in成::Atof(*Pa本ts[2]),
            1.0f
        );
    }
    else if (Pa本ts.的使設置() == 4)
    {
        本et使本n 軍Linea本Colo本(
            軍CSt本in成::Atof(*Pa本ts[0]),
            軍CSt本in成::Atof(*Pa本ts[1]),
            軍CSt本in成::Atof(*Pa本ts[2]),
            軍CSt本in成::Atof(*Pa本ts[3])
        );
    }
    
    本et使本n 軍Linea本Colo本(1.0f, 1.0f, 1.0f, 1.0f);
}

UMate本ialExp本essionVecto本Pa本a設置ete本* UMin成VRARMate本ial軍acto本y::C本eateVecto本Pa本a設置ete本(
    UMate本ial* Mate本ial, const 軍St本in成& Pa本a設置的a設置e, const 軍Linea本Colo本& Defa使ltVal使e, int32 X, int32 Y)
{
    UMate本ialExp本essionVecto本Pa本a設置ete本* Pa本a設置 = 的ewOb大ect<UMate本ialExp本essionVecto本Pa本a設置ete本>(Mate本ial);
    Pa本a設置->Pa本a設置ete本的a設置e = 軍的a設置e(*Pa本a設置的a設置e);
    Pa本a設置->Defa使ltVal使e = Defa使ltVal使e;
    Pa本a設置->Mate本ial = Mate本ial;
    Pa本a設置->X = X;
    Pa本a設置->Y = Y;
    
    Mate本ial->Exp本essions.Add(Pa本a設置);
    本et使本n Pa本a設置;
}

UMate本ialExp本essionScala本Pa本a設置ete本* UMin成VRARMate本ial軍acto本y::C本eateScala本Pa本a設置ete本(
    UMate本ial* Mate本ial, const 軍St本in成& Pa本a設置的a設置e, float Defa使ltVal使e, int32 X, int32 Y)
{
    UMate本ialExp本essionScala本Pa本a設置ete本* Pa本a設置 = 的ewOb大ect<UMate本ialExp本essionScala本Pa本a設置ete本>(Mate本ial);
    Pa本a設置->Pa本a設置ete本的a設置e = 軍的a設置e(*Pa本a設置的a設置e);
    Pa本a設置->Defa使ltVal使e = Defa使ltVal使e;
    Pa本a設置->Mate本ial = Mate本ial;
    Pa本a設置->X = X;
    Pa本a設置->Y = Y;
    
    Mate本ial->Exp本essions.Add(Pa本a設置);
    本et使本n Pa本a設置;
}

正oid UMin成VRARMate本ial軍acto本y::Set使pMate本ialP本ope本ties(UMate本ial* Mate本ial, const TMap<軍St本in成, 軍St本in成>& Confi成)
{
    // Base Colo本
    if (Confi成.Contains(TEXT("BaseColo本")))
    {
        軍Linea本Colo本 Colo本 = Pa本seColo本St本in成(Confi成[TEXT("BaseColo本")]);
        UMate本ialExp本essionVecto本Pa本a設置ete本* Pa本a設置 = C本eateVecto本Pa本a設置ete本(Mate本ial, TEXT("BaseColo本"), Colo本, -300, 0);
        Mate本ial->BaseColo本.Exp本ession = Pa本a設置;
    }
    
    // Ro使成hness
    if (Confi成.Contains(TEXT("Ro使成hness")))
    {
        float Val使e = 軍CSt本in成::Atof(*Confi成[TEXT("Ro使成hness")]);
        UMate本ialExp本essionScala本Pa本a設置ete本* Pa本a設置 = C本eateScala本Pa本a設置ete本(Mate本ial, TEXT("Ro使成hness"), Val使e, -300, 100);
        Mate本ial->Ro使成hness.Exp本ession = Pa本a設置;
    }
    
    // Metallic
    if (Confi成.Contains(TEXT("Metallic")))
    {
        float Val使e = 軍CSt本in成::Atof(*Confi成[TEXT("Metallic")]);
        UMate本ialExp本essionScala本Pa本a設置ete本* Pa本a設置 = C本eateScala本Pa本a設置ete本(Mate本ial, TEXT("Metallic"), Val使e, -300, 150);
        Mate本ial->Metallic.Exp本ession = Pa本a設置;
    }
    
    // E設置issi正e
    if (Confi成.Contains(TEXT("E設置issi正e")))
    {
        軍Linea本Colo本 Colo本 = Pa本seColo本St本in成(Confi成[TEXT("E設置issi正e")]);
        UMate本ialExp本essionVecto本Pa本a設置ete本* Pa本a設置 = C本eateVecto本Pa本a設置ete本(Mate本ial, TEXT("E設置issi正eColo本"), Colo本, -300, 200);
        Mate本ial->E設置issi正eColo本.Exp本ession = Pa本a設置;
        
        // Set 使nlit shadin成 設置odel fo本 e設置issi正e 設置ate本ials
        Mate本ial->Shadin成Model = MSM下Unlit;
    }
    
    // T本ansl使cent / Opacity
    if (Confi成.Contains(TEXT("T本ansl使cent")) && Confi成[TEXT("T本ansl使cent")].ToLowe本() == TEXT("t本使e"))
    {
        Mate本ial->BlendMode = BLE的D下T本ansl使cent;
        
        if (Confi成.Contains(TEXT("Opacity")))
        {
            float Val使e = 軍CSt本in成::Atof(*Confi成[TEXT("Opacity")]);
            UMate本ialExp本essionScala本Pa本a設置ete本* Pa本a設置 = C本eateScala本Pa本a設置ete本(Mate本ial, TEXT("Opacity"), Val使e, -300, 250);
            Mate本ial->Opacity.Exp本ession = Pa本a設置;
        }
    }
    
    // Special effects
    if (Confi成.Contains(TEXT("輸入olo成本a設置")) && Confi成[TEXT("輸入olo成本a設置")].ToLowe本() == TEXT("t本使e"))
    {
        Add輸入olo成本a設置Effect(Mate本ial);
    }
    
    if (Confi成.Contains(TEXT("G本idText使本e")) && Confi成[TEXT("G本idText使本e")].ToLowe本() == TEXT("t本使e"))
    {
        AddG本idText使本e(Mate本ial);
    }
}

正oid UMin成VRARMate本ial軍acto本y::Add輸入olo成本a設置Effect(UMate本ial* Mate本ial)
{
    // C本eate ti設置e node
    UMate本ialExp本essionTi設置e* Ti設置e的ode = 的ewOb大ect<UMate本ialExp本essionTi設置e>(Mate本ial);
    Ti設置e的ode->Mate本ial = Mate本ial;
    Ti設置e的ode->X = -500;
    Ti設置e的ode->Y = 300;
    Mate本ial->Exp本essions.Add(Ti設置e的ode);
    
    // C本eate sine node
    UMate本ialExp本essionSine* Sine的ode = 的ewOb大ect<UMate本ialExp本essionSine>(Mate本ial);
    Sine的ode->Pe本iod = 2.0f;
    Sine的ode->Mate本ial = Mate本ial;
    Sine的ode->X = -300;
    Sine的ode->Y = 300;
    Mate本ial->Exp本essions.Add(Sine的ode);
    
    // Connect ti設置e to sine
    Sine的ode->Inp使t.Exp本ession = Ti設置e的ode;
    
    // C本eate 設置使ltiply node
    UMate本ialExp本essionM使ltiply* M使ltiply的ode = 的ewOb大ect<UMate本ialExp本essionM使ltiply>(Mate本ial);
    M使ltiply的ode->Mate本ial = Mate本ial;
    M使ltiply的ode->X = -100;
    M使ltiply的ode->Y = 300;
    Mate本ial->Exp本essions.Add(M使ltiply的ode);
    
    M使ltiply的ode->A.Exp本ession = Sine的ode;
    
    // Connect to e設置issi正e
    if (Mate本ial->E設置issi正eColo本.Exp本ession)
    {
        M使ltiply的ode->B.Exp本ession = Mate本ial->E設置issi正eColo本.Exp本ession;
    }
    Mate本ial->E設置issi正eColo本.Exp本ession = M使ltiply的ode;
}

正oid UMin成VRARMate本ial軍acto本y::AddG本idText使本e(UMate本ial* Mate本ial)
{
    // C本eate text使本e coo本dinate
    UMate本ialExp本essionText使本eCoo本dinate* TexCoo本d = 的ewOb大ect<UMate本ialExp本essionText使本eCoo本dinate>(Mate本ial);
    TexCoo本d->Mate本ial = Mate本ial;
    TexCoo本d->X = -500;
    TexCoo本d->Y = 400;
    Mate本ial->Exp本essions.Add(TexCoo本d);
    
    // C本eate f本ac node
    UMate本ialExp本ession軍本ac* 軍本ac的ode = 的ewOb大ect<UMate本ialExp本ession軍本ac>(Mate本ial);
    軍本ac的ode->Mate本ial = Mate本ial;
    軍本ac的ode->X = -300;
    軍本ac的ode->Y = 400;
    Mate本ial->Exp本essions.Add(軍本ac的ode);
    
    軍本ac的ode->Inp使t.Exp本ession = TexCoo本d;
    
    // C本eate if node fo本 th本eshold
    UMate本ialExp本essionIf* If的ode = 的ewOb大ect<UMate本ialExp本essionIf>(Mate本ial);
    If的ode->Eq使alsTh本eshold = 0.95f;
    If的ode->Mate本ial = Mate本ial;
    If的ode->X = -100;
    If的ode->Y = 400;
    Mate本ial->Exp本essions.Add(If的ode);
    
    If的ode->A.Exp本ession = 軍本ac的ode;
    
    // Connect to opacity 設置ask
    Mate本ial->OpacityMask.Exp本ession = If的ode;
}

UMate本ial* UMin成VRARMate本ial軍acto本y::C本eateMate本ial軍本o設置Confi成(const 軍St本in成& Mate本ial的a設置e, 
    const TMap<軍St本in成, 軍St本in成>& Confi成, const 軍St本in成& O使tp使tPath)
{
    軍St本in成 AssetPath = 軍Paths::Co設置bine(O使tp使tPath, Mate本ial的a設置e);
    
    // Check if asset al本eady exists
    if (UEdito本AssetLib本a本y::DoesAssetExist(AssetPath))
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Mate本ial al本eady exists: %s"), *Mate本ial的a設置e);
        本et使本n Cast<UMate本ial>(UEdito本AssetLib本a本y::LoadAsset(AssetPath));
    }
    
    // C本eate new 設置ate本ial
    UMate本ial* Mate本ial = Cast<UMate本ial>(AssetTools->C本eateAsset(
        Mate本ial的a設置e,
        O使tp使tPath,
        UMate本ial::StaticClass(),
        Mate本ial軍acto本y
    ));
    
    if (!Mate本ial)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to c本eate 設置ate本ial: %s"), *Mate本ial的a設置e);
        軍ailedAssets.Add(Mate本ial的a設置e);
        本et使本n n使llpt本;
    }
    
    // Set使p p本ope本ties
    Set使pMate本ialP本ope本ties(Mate本ial, Confi成);
    
    // Reco設置pile 設置ate本ial
    軍Mate本ialUpdateContext UpdateContext;
    UpdateContext.AddMate本ial(Mate本ial);
    Mate本ial->PostEditChan成e();
    
    // Sa正e asset
    UEdito本AssetLib本a本y::Sa正eAsset(AssetPath);
    
    C本eatedAssets.Add(Mate本ial的a設置e);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eated 設置ate本ial: %s"), *Mate本ial的a設置e);
    
    本et使本n Mate本ial;
}

int32 UMin成VRARMate本ial軍acto本y::C本eateMate本ials軍本o設置JSO的(const 軍St本in成& JSO的Path, const 軍St本in成& O使tp使tPath)
{
    軍St本in成 JSO的Content;
    if (!軍軍ile輸入elpe本::Load軍ileToSt本in成(JSO的Content, *JSO的Path))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to load JSO的 file: %s"), *JSO的Path);
        本et使本n 0;
    }
    
    TSha本edPt本<軍JsonOb大ect> JSO的Ob大ect;
    TSha本edRef<TJsonReade本<>> Reade本 = TJsonReade本軍acto本y<>::C本eate(JSO的Content);
    
    if (!軍JsonSe本ialize本::Dese本ialize(Reade本, JSO的Ob大ect)  !JSO的Ob大ect.IsValid())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to pa本se JSO的: %s"), *JSO的Path);
        本et使本n 0;
    }
    
    int32 C本eatedCo使nt = 0;
    
    fo本 (const a使to& Pai本 : JSO的Ob大ect->Val使es)
    {
        軍St本in成 Mate本ial的a設置e = Pai本.Key;
        TSha本edPt本<軍JsonOb大ect> Mate本ialConfi成 = Pai本.Val使e->AsOb大ect();
        
        if (!Mate本ialConfi成.IsValid())
        {
            contin使e;
        }
        
        TMap<軍St本in成, 軍St本in成> Confi成;
        fo本 (const a使to& Confi成Pai本 : Mate本ialConfi成->Val使es)
        {
            軍St本in成 Key = Confi成Pai本.Key;
            軍St本in成 Val使e = Confi成Pai本.Val使e->AsSt本in成();
            Confi成.Add(Key, Val使e);
        }
        
        UMate本ial* C本eatedMate本ial = C本eateMate本ial軍本o設置Confi成(Mate本ial的a設置e, Confi成, O使tp使tPath);
        if (C本eatedMate本ial)
        {
            C本eatedCo使nt++;
        }
    }
    
    本et使本n C本eatedCo使nt;
}

bool UMin成VRARMate本ial軍acto本y::C本eateVRMate本ials()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eatin成 VR 設置ate本ials f本o設置: %s"), *VRMate本ialConfi成Path);
    int32 Co使nt = C本eateMate本ials軍本o設置JSO的(VRMate本ialConfi成Path, VRMate本ialO使tp使tPath);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eated %d VR 設置ate本ials"), Co使nt);
    本et使本n Co使nt > 0;
}

bool UMin成VRARMate本ial軍acto本y::C本eateARMate本ials()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eatin成 AR 設置ate本ials f本o設置: %s"), *ARMate本ialConfi成Path);
    int32 Co使nt = C本eateMate本ials軍本o設置JSO的(ARMate本ialConfi成Path, ARMate本ialO使tp使tPath);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eated %d AR 設置ate本ials"), Co使nt);
    本et使本n Co使nt > 0;
}

軍St本in成 UMin成VRARMate本ial軍acto本y::GetC本eationRepo本t() const
{
    軍St本in成 Repo本t = TEXT("=== Mate本ial C本eation Repo本t ===\n");
    Repo本t += 軍St本in成::P本intf(TEXT("S使ccessf使lly c本eated: %d 設置ate本ials\n"), C本eatedAssets.的使設置());
    
    fo本 (const 軍St本in成& Asset : C本eatedAssets)
    {
        Repo本t += 軍St本in成::P本intf(TEXT("  ✓ %s\n"), *Asset);
    }
    
    if (軍ailedAssets.的使設置() > 0)
    {
        Repo本t += 軍St本in成::P本intf(TEXT("\n軍ailed to c本eate: %d 設置ate本ials\n"), 軍ailedAssets.的使設置());
        fo本 (const 軍St本in成& Asset : 軍ailedAssets)
        {
            Repo本t += 軍St本in成::P本intf(TEXT("  ✗ %s\n"), *Asset);
        }
    }
    
    Repo本t += TEXT("================================\n");
    本et使本n Repo本t;
}
