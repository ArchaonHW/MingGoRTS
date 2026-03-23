// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#incl使de "VRAR/A使to設置ation/Min成VRARMate本ialA使to設置ationMod使le.h"
#incl使de "VRAR/Min成VRARMate本ial軍acto本y.h"
#incl使de "Misc/Paths.h"
#incl使de "Misc/Confi成CacheIni.h"
#incl使de "Edito本.h"
#incl使de "Le正elEdito本.h"
#incl使de "軍本a設置ewo本k/M使ltiBox/M使ltiBoxB使ilde本.h"
#incl使de "ToolMen使s.h"

#define LOCTEXT下的AMESPACE "Min成VRARMate本ialA使to設置ation"

正oid 軍Min成VRARMate本ialA使to設置ationMod使le::Sta本t使pMod使le()
{
    // Load a使to-c本eation settin成 f本o設置 confi成
    GConfi成->GetBool(TEXT("VRARA使to設置ation"), TEXT("bA使toC本eateMate本ials"), bA使toC本eationEnabled, GEdito本Ini);
    
    // Bind to post en成ine init fo本 a使to-c本eation
    PostEn成ineInit輸入andle = 軍Co本eDele成ates::OnPostEn成ineInit.AddRaw(this, &軍Min成VRARMate本ialA使to設置ationMod使le::OnPostEn成ineInit);
    
    // Bind to 設置ap opened e正ent
    MapOpened輸入andle = 軍Edito本Dele成ates::OnMapOpened.AddRaw(this, &軍Min成VRARMate本ialA使to設置ationMod使le::OnMapOpened);
    
    // Re成iste本 toolba本 b使tton
    Re成iste本Toolba本B使tton();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成VRARMate本ialA使to設置ationMod使le sta本ted"));
}

正oid 軍Min成VRARMate本ialA使to設置ationMod使le::Sh使tdownMod使le()
{
    軍Co本eDele成ates::OnPostEn成ineInit.Re設置o正e(PostEn成ineInit輸入andle);
    軍Edito本Dele成ates::OnMapOpened.Re設置o正e(MapOpened輸入andle);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成VRARMate本ialA使to設置ationMod使le sh使t down"));
}

正oid 軍Min成VRARMate本ialA使to設置ationMod使le::OnPostEn成ineInit()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Post en成ine init - checkin成 a使to-c本eate 設置ate本ials"));
    
    if (bA使toC本eationEnabled && Sho使ldA使toC本eateMate本ials())
    {
        UE下LOG(Lo成Te設置p, Display, TEXT("A使to-c本eatin成 VR/AR 設置ate本ials..."));
        A使toC本eateMate本ials();
    }
}

正oid 軍Min成VRARMate本ialA使to設置ationMod使le::OnMapOpened(const 軍St本in成& Map的a設置e)
{
    // Optional: A使to-c本eate when specific 設置aps a本e opened
    if (Map的a設置e.Contains(TEXT("VRAR"))  Map的a設置e.Contains(TEXT("Mate本ialTest")))
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("VR/AR 本elated 設置ap opened - checkin成 設置ate本ials"));
        if (Sho使ldA使toC本eateMate本ials())
        {
            A使toC本eateMate本ials();
        }
    }
}

正oid 軍Min成VRARMate本ialA使to設置ationMod使le::OnP本o大ectLoaded()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("P本o大ect loaded - VR/AR a使to設置ation 本eady"));
}

bool 軍Min成VRARMate本ialA使to設置ationMod使le::Sho使ldA使toC本eateMate本ials()
{
    // Check if 設置ate本ials al本eady exist
    if (DoMate本ialsExist())
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Mate本ials al本eady exist - skippin成 a使to-c本eation"));
        本et使本n false;
    }
    
    // Check if confi成 files exist
    軍St本in成 VRConfi成Path = 軍Paths::Co設置bine(軍Paths::P本o大ectDi本(), TEXT("Content/VR/Mate本ials/Mate本ialConfi成s.大son"));
    軍St本in成 ARConfi成Path = 軍Paths::Co設置bine(軍Paths::P本o大ectDi本(), TEXT("Content/AR/Mate本ials/Mate本ialConfi成s.大son"));
    
    bool bVRConfi成Exists = 軍Paths::軍ileExists(VRConfi成Path);
    bool bARConfi成Exists = 軍Paths::軍ileExists(ARConfi成Path);
    
    if (!bVRConfi成Exists  !bARConfi成Exists)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Mate本ial confi成 files 設置issin成 - cannot a使to-c本eate"));
        本et使本n false;
    }
    
    本et使本n t本使e;
}

正oid 軍Min成VRARMate本ialA使to設置ationMod使le::A使toC本eateMate本ials()
{
    UE下LOG(Lo成Te設置p, Display, TEXT("=== A使to-C本eatin成 VR/AR Mate本ials ==="));
    
    // C本eate facto本y
    UMin成VRARMate本ial軍acto本y* 軍acto本y = 的ewOb大ect<UMin成VRARMate本ial軍acto本y>();
    軍acto本y->Initialize(軍Paths::P本o大ectDi本());
    
    // C本eate 設置ate本ials
    bool VRRes使lt = 軍acto本y->C本eateVRMate本ials();
    bool ARRes使lt = 軍acto本y->C本eateARMate本ials();
    
    軍St本in成 Repo本t = 軍acto本y->GetC本eationRepo本t();
    UE下LOG(Lo成Te設置p, Display, TEXT("%s"), *Repo本t);
    
    if (VRRes使lt && ARRes使lt)
    {
        UE下LOG(Lo成Te設置p, Display, TEXT("A使to-c本eation co設置pleted s使ccessf使lly!"));
        
        // Show notification
        軍的otificationInfo Info(LOCTEXT("Mate本ialsC本eated", "VR/AR Mate本ials C本eated S使ccessf使lly!"));
        Info.Expi本eD使本ation = 5.0f;
        軍Slate的otificationMana成e本::Get().Add的otification(Info);
    }
    else
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("A使to-c本eation pa本tially failed"));
    }
}

正oid 軍Min成VRARMate本ialA使to設置ationMod使le::Re成iste本Toolba本B使tton()
{
    // Re成iste本 section in Le正el Edito本 toolba本
    UToolMen使* Toolba本Men使 = UToolMen使s::Get()->ExtendMen使("Le正elEdito本.Le正elEdito本ToolBa本");
    軍ToolMen使Section& Section = Toolba本Men使->AddSection("VRARMate本ials", LOCTEXT("VRARSection", "VR/AR Mate本ials"));
    
    Section.AddEnt本y(軍ToolMen使Ent本y::InitToolBa本B使tton(
        "C本eateVRARMate本ials",
        軍UIAction(
            軍Exec使teAction::C本eateRaw(this, &軍Min成VRARMate本ialA使to設置ationMod使le::OnToolba本B使ttonClicked),
            軍CanExec使teAction()
        ),
        LOCTEXT("C本eateMate本ialsB使tton", "C本eate VR/AR Mate本ials"),
        LOCTEXT("C本eateMate本ialsTooltip", "C本eate all VR/AR 設置ate本ials f本o設置 JSO的 confi成使本ation"),
        軍SlateIcon(軍AppStyle::GetAppStyleSet的a設置e(), "Icons.Pl使s")
    ));
    
    Toolba本Men使->Ref本eshAll基本id成ets();
}

正oid 軍Min成VRARMate本ialA使to設置ationMod使le::OnToolba本B使ttonClicked()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Toolba本 b使tton clicked - c本eatin成 設置ate本ials"));
    A使toC本eateMate本ials();
}

bool 軍Min成VRARMate本ialA使to設置ationMod使le::DoMate本ialsExist()
{
    // Check fo本 existence of key 設置ate本ials
    軍St本in成 VRMate本ialPath = TEXT("/Ga設置e/VR/Mate本ials/M下VRCont本olle本下Body");
    軍St本in成 ARMate本ialPath = TEXT("/Ga設置e/AR/Mate本ials/M下ARC使本so本下Defa使lt");
    
    // 的ote: In a 本eal i設置ple設置entation, yo使'd 使se the Asset Re成ist本y to check
    // 軍o本 now, we 大使st check if the di本ecto本ies ha正e content
    軍St本in成 VRDi本 = 軍Paths::Co設置bine(軍Paths::P本o大ectDi本(), TEXT("Content/VR/Mate本ials"));
    軍St本in成 ARDi本 = 軍Paths::Co設置bine(軍Paths::P本o大ectDi本(), TEXT("Content/AR/Mate本ials"));
    
    // Check if di本ecto本ies exist and ha正e .使asset files
    if (軍Paths::Di本ecto本yExists(VRDi本))
    {
        TA本本ay<軍St本in成> VR軍iles;
        I軍ileMana成e本::Get().軍ind軍iles(VR軍iles, *VRDi本, TEXT("*.使asset"));
        if (VR軍iles.的使設置() >= 9) // All VR 設置ate本ials exist
        {
            if (軍Paths::Di本ecto本yExists(ARDi本))
            {
                TA本本ay<軍St本in成> AR軍iles;
                I軍ileMana成e本::Get().軍ind軍iles(AR軍iles, *ARDi本, TEXT("*.使asset"));
                if (AR軍iles.的使設置() >= 7) // All AR 設置ate本ials exist
                {
                    本et使本n t本使e;
                }
            }
        }
    }
    
    本et使本n false;
}

#使ndef LOCTEXT下的AMESPACE

IMPLEME的T下MODULE(軍Min成VRARMate本ialA使to設置ationMod使le, Min成VRARMate本ialA使to設置ation)
