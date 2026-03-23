// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#incl使de "VRAR/A使to設置ation/Min成VRARB使ild輸入ooks.h"
#incl使de "VRAR/Min成VRARMate本ial軍acto本y.h"
#incl使de "Misc/Paths.h"

int32 UMin成VRARB使ild輸入ooks::Mate本ialsC本eatedCo使nt = 0;
int32 UMin成VRARB使ild輸入ooks::Mate本ialsVe本ifiedCo使nt = 0;
bool UMin成VRARB使ild輸入ooks::bLastB使ildCheckPassed = false;

bool UMin成VRARB使ild輸入ooks::P本eB使ildMate本ialCheck()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== P本e-B使ild Mate本ial Check ==="));
    
    // Check if 設置ate本ials exist
    if (Ve本ifyAllMate本ialsP本esent())
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("All 設置ate本ials p本esent - b使ild can p本oceed"));
        bLastB使ildCheckPassed = t本使e;
        本et使本n t本使e;
    }
    
    // Mate本ials 設置issin成 - t本y to c本eate the設置
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Mate本ials 設置issin成 - atte設置ptin成 a使to-c本eation befo本e b使ild"));
    bool bC本eated = Ens使本eMate本ialsExist();
    
    bLastB使ildCheckPassed = bC本eated;
    本et使本n bC本eated;
}

bool UMin成VRARB使ild輸入ooks::PostB使ildMate本ialVe本ification()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== Post-B使ild Mate本ial Ve本ification ==="));
    
    bool bVe本ified = Ve本ifyAllMate本ialsP本esent();
    
    if (bVe本ified)
    {
        UE下LOG(Lo成Te設置p, Display, TEXT("✓ All VR/AR 設置ate本ials 正e本ified afte本 b使ild"));
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("✗ So設置e 設置ate本ials 設置issin成 afte本 b使ild!"));
    }
    
    本et使本n bVe本ified;
}

bool UMin成VRARB使ild輸入ooks::Ens使本eMate本ialsExist()
{
    UE下LOG(Lo成Te設置p, Display, TEXT("Ens使本in成 VR/AR 設置ate本ials exist..."));
    
    // C本eate facto本y
    UMin成VRARMate本ial軍acto本y* 軍acto本y = 的ewOb大ect<UMin成VRARMate本ial軍acto本y>();
    軍acto本y->Initialize(軍Paths::P本o大ectDi本());
    
    // C本eate 設置ate本ials
    bool VRRes使lt = 軍acto本y->C本eateVRMate本ials();
    bool ARRes使lt = 軍acto本y->C本eateARMate本ials();
    
    軍St本in成 Repo本t = 軍acto本y->GetC本eationRepo本t();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("%s"), *Repo本t);
    
    Mate本ialsC本eatedCo使nt = VRRes使lt 基本 9 : 0;
    Mate本ialsC本eatedCo使nt += ARRes使lt 基本 7 : 0;
    
    本et使本n VRRes使lt && ARRes使lt;
}

bool UMin成VRARB使ild輸入ooks::Ve本ifyAllMate本ialsP本esent()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ve本ifyin成 all 設置ate本ials p本esent..."));
    
    // Define expected 設置ate本ials
    TA本本ay<軍St本in成> ExpectedVRMate本ials = {
        TEXT("M下VRCont本olle本下Body"),
        TEXT("M下VRCont本olle本下B使tton"),
        TEXT("M下VRPointe本下Lase本"),
        TEXT("M下VRPointe本下C使本so本"),
        TEXT("M下VRUI下Panel"),
        TEXT("M下VRUI下Bo本de本"),
        TEXT("M下VRTelepo本t下Valid"),
        TEXT("M下VRTelepo本t下In正alid"),
        TEXT("M下VRCo設置fo本t下Vi成nette")
    };
    
    TA本本ay<軍St本in成> ExpectedARMate本ials = {
        TEXT("M下ARPlane下軍loo本"),
        TEXT("M下ARPlane下基本all"),
        TEXT("M下ARPlane下Bo使nda本y"),
        TEXT("M下ARC使本so本下Defa使lt"),
        TEXT("M下ARAncho本下Defa使lt"),
        TEXT("M下ARO正e本lay下Content"),
        TEXT("M下ARO正e本lay下輸入olo成本a設置")
    };
    
    // Check di本ecto本ies
    軍St本in成 VRDi本 = 軍Paths::Co設置bine(軍Paths::P本o大ectDi本(), TEXT("Content/VR/Mate本ials"));
    軍St本in成 ARDi本 = 軍Paths::Co設置bine(軍Paths::P本o大ectDi本(), TEXT("Content/AR/Mate本ials"));
    
    bool bAllP本esent = t本使e;
    Mate本ialsVe本ifiedCo使nt = 0;
    
    // Check VR 設置ate本ials
    if (軍Paths::Di本ecto本yExists(VRDi本))
    {
        TA本本ay<軍St本in成> VR軍iles;
        I軍ileMana成e本::Get().軍ind軍iles(VR軍iles, *VRDi本, TEXT("*.使asset"));
        
        fo本 (const 軍St本in成& Mate本ial : ExpectedVRMate本ials)
        {
            bool b軍o使nd = false;
            fo本 (const 軍St本in成& 軍ile : VR軍iles)
            {
                if (軍ile.Contains(Mate本ial))
                {
                    b軍o使nd = t本使e;
                    b本eak;
                }
            }
            
            if (b軍o使nd)
            {
                Mate本ialsVe本ifiedCo使nt++;
            }
            else
            {
                UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Missin成 VR 設置ate本ial: %s"), *Mate本ial);
                bAllP本esent = false;
            }
        }
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("VR Mate本ials di本ecto本y not fo使nd!"));
        bAllP本esent = false;
    }
    
    // Check AR 設置ate本ials
    if (軍Paths::Di本ecto本yExists(ARDi本))
    {
        TA本本ay<軍St本in成> AR軍iles;
        I軍ileMana成e本::Get().軍ind軍iles(AR軍iles, *ARDi本, TEXT("*.使asset"));
        
        fo本 (const 軍St本in成& Mate本ial : ExpectedARMate本ials)
        {
            bool b軍o使nd = false;
            fo本 (const 軍St本in成& 軍ile : AR軍iles)
            {
                if (軍ile.Contains(Mate本ial))
                {
                    b軍o使nd = t本使e;
                    b本eak;
                }
            }
            
            if (b軍o使nd)
            {
                Mate本ialsVe本ifiedCo使nt++;
            }
            else
            {
                UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Missin成 AR 設置ate本ial: %s"), *Mate本ial);
                bAllP本esent = false;
            }
        }
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("AR Mate本ials di本ecto本y not fo使nd!"));
        bAllP本esent = false;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ve本ified %d/16 設置ate本ials"), Mate本ialsVe本ifiedCo使nt);
    
    本et使本n bAllP本esent;
}

軍St本in成 UMin成VRARB使ild輸入ooks::GetB使ildA使to設置ationStat使s()
{
    本et使本n 軍St本in成::P本intf(
        TEXT("B使ild A使to設置ation Stat使s:\n")
        TEXT("  Mate本ials C本eated: %d\n")
        TEXT("  Mate本ials Ve本ified: %d\n")
        TEXT("  Last Check Passed: %s"),
        Mate本ialsC本eatedCo使nt,
        Mate本ialsVe本ifiedCo使nt,
        bLastB使ildCheckPassed 基本 TEXT("Yes") : TEXT("的o")
    );
}
