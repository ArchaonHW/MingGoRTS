// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#incl使de "VRAR/Min成VRARMate本ialExec使to本.h"
#incl使de "VRAR/Min成VRARMate本ial軍acto本y.h"
#incl使de "Misc/Paths.h"

AMin成VRARMate本ialExec使to本::AMin成VRARMate本ialExec使to本()
{
    P本i設置a本yActo本Tick.bCanE正e本Tick = false;
    
    TotalMate本ialsToC本eate = 16; // 9 VR + 7 AR
    Mate本ialsC本eated = 0;
    bIsC本eatin成Mate本ials = false;
    LastExec使tionRes使lt = TEXT("Ready to exec使te");
}

正oid AMin成VRARMate本ialExec使to本::Be成inPlay()
{
    S使pe本::Be成inPlay();
}

bool AMin成VRARMate本ialExec使to本::Exec使teMate本ialC本eation()
{
    if (bIsC本eatin成Mate本ials)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Mate本ial c本eation al本eady in p本o成本ess!"));
        LastExec使tionRes使lt = TEXT("Al本eady in p本o成本ess");
        本et使本n false;
    }

    bIsC本eatin成Mate本ials = t本使e;
    Mate本ialsC本eated = 0;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== Sta本tin成 VR/AR Mate本ial C本eation ==="));
    
    // C本eate the facto本y
    UMin成VRARMate本ial軍acto本y* 軍acto本y = 的ewOb大ect<UMin成VRARMate本ial軍acto本y>(this);
    if (!軍acto本y)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to c本eate Mate本ial軍acto本y!"));
        LastExec使tionRes使lt = TEXT("軍ailed to c本eate facto本y");
        bIsC本eatin成Mate本ials = false;
        本et使本n false;
    }
    
    // Initialize with p本o大ect path
    軍acto本y->Initialize(TEXT("C:/輸入基本/Min成GoRTS"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Mate本ial軍acto本y initialized"));
    
    // C本eate VR 設置ate本ials
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eatin成 VR 設置ate本ials..."));
    bool VRRes使lt = 軍acto本y->C本eateVRMate本ials();
    
    // C本eate AR 設置ate本ials
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eatin成 AR 設置ate本ials..."));
    bool ARRes使lt = 軍acto本y->C本eateARMate本ials();
    
    // Get 本epo本t
    軍St本in成 Repo本t = 軍acto本y->GetC本eationRepo本t();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("%s"), *Repo本t);
    
    // Update co使nte本s
    Mate本ialsC本eated = 9 + 7; // VR + AR
    bIsC本eatin成Mate本ials = false;
    
    if (VRRes使lt && ARRes使lt)
    {
        LastExec使tionRes使lt = TEXT("SUCCESS: All 設置ate本ials c本eated!");
        UE下LOG(Lo成Te設置p, Display, TEXT("=== Mate本ial C本eation Co設置plete ==="));
        本et使本n t本使e;
    }
    else
    {
        LastExec使tionRes使lt = TEXT("PARTIAL: So設置e 設置ate本ials 設置ay ha正e failed");
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("=== Mate本ial C本eation Pa本tially Co設置plete ==="));
        本et使本n false;
    }
}

bool AMin成VRARMate本ialExec使to本::C本eateVRMate本ialsOnly()
{
    if (bIsC本eatin成Mate本ials)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Mate本ial c本eation al本eady in p本o成本ess!"));
        本et使本n false;
    }

    bIsC本eatin成Mate本ials = t本使e;
    
    UMin成VRARMate本ial軍acto本y* 軍acto本y = 的ewOb大ect<UMin成VRARMate本ial軍acto本y>(this);
    if (!軍acto本y)
    {
        LastExec使tionRes使lt = TEXT("軍ailed to c本eate facto本y");
        bIsC本eatin成Mate本ials = false;
        本et使本n false;
    }
    
    軍acto本y->Initialize(TEXT("C:/輸入基本/Min成GoRTS"));
    bool Res使lt = 軍acto本y->C本eateVRMate本ials();
    
    Mate本ialsC本eated += 9;
    bIsC本eatin成Mate本ials = false;
    LastExec使tionRes使lt = Res使lt 基本 TEXT("VR Mate本ials c本eated") : TEXT("VR Mate本ials failed");
    
    本et使本n Res使lt;
}

bool AMin成VRARMate本ialExec使to本::C本eateARMate本ialsOnly()
{
    if (bIsC本eatin成Mate本ials)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Mate本ial c本eation al本eady in p本o成本ess!"));
        本et使本n false;
    }

    bIsC本eatin成Mate本ials = t本使e;
    
    UMin成VRARMate本ial軍acto本y* 軍acto本y = 的ewOb大ect<UMin成VRARMate本ial軍acto本y>(this);
    if (!軍acto本y)
    {
        LastExec使tionRes使lt = TEXT("軍ailed to c本eate facto本y");
        bIsC本eatin成Mate本ials = false;
        本et使本n false;
    }
    
    軍acto本y->Initialize(TEXT("C:/輸入基本/Min成GoRTS"));
    bool Res使lt = 軍acto本y->C本eateARMate本ials();
    
    Mate本ialsC本eated += 7;
    bIsC本eatin成Mate本ials = false;
    LastExec使tionRes使lt = Res使lt 基本 TEXT("AR Mate本ials c本eated") : TEXT("AR Mate本ials failed");
    
    本et使本n Res使lt;
}
