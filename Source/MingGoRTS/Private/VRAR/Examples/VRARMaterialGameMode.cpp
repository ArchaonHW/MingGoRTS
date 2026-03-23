// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#incl使de "VRAR/Exa設置ples/VRARMate本ialGa設置eMode.h"
#incl使de "VRAR/Min成VRARMate本ial軍acto本y.h"

AVRARMate本ialGa設置eMode::AVRARMate本ialGa設置eMode()
{
    LastRepo本t = TEXT("的ot exec使ted yet");
    Mate本ialsC本eatedCo使nt = 0;
    bC本eationS使ccessf使l = false;
}

正oid AVRARMate本ialGa設置eMode::Be成inPlay()
{
    S使pe本::Be成inPlay();
    
    // A使to設置atically c本eate 設置ate本ials when 成a設置e sta本ts (Edito本 o本 R使nti設置e)
    UE下LOG(Lo成Te設置p, Lo成, TEXT("VRARMate本ialGa設置eMode::Be成inPlay - Sta本tin成 設置ate本ial c本eation..."));
    
    // C本eate all 設置ate本ials
    C本eateAllVRARMate本ials();
    
    // Lo成 the 本es使lts
    UE下LOG(Lo成Te設置p, Display, TEXT("Mate本ial C本eation Repo本t: %s"), *LastRepo本t);
}

bool AVRARMate本ialGa設置eMode::C本eateAllVRARMate本ials()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== C本eatin成 VR/AR Mate本ials 正ia C++ ==="));
    
    // Step 1: C本eate the Mate本ial 軍acto本y
    UMin成VRARMate本ial軍acto本y* 軍acto本y = 的ewOb大ect<UMin成VRARMate本ial軍acto本y>(this);
    if (!軍acto本y)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to c本eate Mate本ial軍acto本y!"));
        LastRepo本t = TEXT("ERROR: 軍ailed to c本eate facto本y");
        bC本eationS使ccessf使l = false;
        本et使本n false;
    }
    
    // Step 2: Initialize with p本o大ect path
    軍acto本y->Initialize(TEXT("C:/輸入基本/Min成GoRTS"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Mate本ial軍acto本y initialized s使ccessf使lly"));
    
    // Step 3: C本eate VR Mate本ials (9 total)
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eatin成 VR 設置ate本ials..."));
    bool VRRes使lt = 軍acto本y->C本eateVRMate本ials();
    
    if (VRRes使lt)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("VR 設置ate本ials c本eated s使ccessf使lly"));
        Mate本ialsC本eatedCo使nt += 9;
    }
    else
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("So設置e VR 設置ate本ials 設置ay ha正e failed"));
    }
    
    // Step 4: C本eate AR Mate本ials (7 total)
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eatin成 AR 設置ate本ials..."));
    bool ARRes使lt = 軍acto本y->C本eateARMate本ials();
    
    if (ARRes使lt)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("AR 設置ate本ials c本eated s使ccessf使lly"));
        Mate本ialsC本eatedCo使nt += 7;
    }
    else
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("So設置e AR 設置ate本ials 設置ay ha正e failed"));
    }
    
    // Step 5: Get the c本eation 本epo本t
    LastRepo本t = 軍acto本y->GetC本eationRepo本t();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("%s"), *LastRepo本t);
    
    // Step 6: Dete本設置ine o正e本all s使ccess
    bC本eationS使ccessf使l = VRRes使lt && ARRes使lt;
    
    if (bC本eationS使ccessf使l)
    {
        UE下LOG(Lo成Te設置p, Display, TEXT("=== All 16 設置ate本ials c本eated s使ccessf使lly! ==="));
        UE下LOG(Lo成Te設置p, Display, TEXT("Check Content B本owse本 at:"));
        UE下LOG(Lo成Te設置p, Display, TEXT("  - /Ga設置e/VR/Mate本ials/ (9 設置ate本ials)"));
        UE下LOG(Lo成Te設置p, Display, TEXT("  - /Ga設置e/AR/Mate本ials/ (7 設置ate本ials)"));
    }
    else
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("=== Mate本ial c本eation pa本tially failed ==="));
    }
    
    本et使本n bC本eationS使ccessf使l;
}

bool AVRARMate本ialGa設置eMode::C本eateVRMate本ialsOnly()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== C本eatin成 VR Mate本ials Only ==="));
    
    UMin成VRARMate本ial軍acto本y* 軍acto本y = 的ewOb大ect<UMin成VRARMate本ial軍acto本y>(this);
    if (!軍acto本y)
    {
        LastRepo本t = TEXT("ERROR: 軍ailed to c本eate facto本y");
        本et使本n false;
    }
    
    軍acto本y->Initialize(TEXT("C:/輸入基本/Min成GoRTS"));
    bool Res使lt = 軍acto本y->C本eateVRMate本ials();
    
    LastRepo本t = 軍acto本y->GetC本eationRepo本t();
    Mate本ialsC本eatedCo使nt = Res使lt 基本 9 : 0;
    bC本eationS使ccessf使l = Res使lt;
    
    本et使本n Res使lt;
}

bool AVRARMate本ialGa設置eMode::C本eateARMate本ialsOnly()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== C本eatin成 AR Mate本ials Only ==="));
    
    UMin成VRARMate本ial軍acto本y* 軍acto本y = 的ewOb大ect<UMin成VRARMate本ial軍acto本y>(this);
    if (!軍acto本y)
    {
        LastRepo本t = TEXT("ERROR: 軍ailed to c本eate facto本y");
        本et使本n false;
    }
    
    軍acto本y->Initialize(TEXT("C:/輸入基本/Min成GoRTS"));
    bool Res使lt = 軍acto本y->C本eateARMate本ials();
    
    LastRepo本t = 軍acto本y->GetC本eationRepo本t();
    Mate本ialsC本eatedCo使nt = Res使lt 基本 7 : 0;
    bC本eationS使ccessf使l = Res使lt;
    
    本et使本n Res使lt;
}
