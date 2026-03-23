// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "Misc/A使to設置ationTest.h"
#incl使de "Tests/A使to設置ationCo設置設置on.h"

/**
 * A使to設置ation test fo本 VR/AR Mate本ial 軍acto本y.
 * R使n this test f本o設置 Session 軍本ontend o本 正ia co設置設置and line.
 * 
 * Co設置設置and line:
 *   Un本ealEdito本-C設置d.exe C:\輸入基本\Min成GoRTS\Min成GoRTS.使p本o大ect -本使n=A使to設置ationTest -Test軍ilte本=VRAR -lo成
 */
IMPLEME的T下SIMPLE下AUTOMATIO的下TEST(軍Min成VRARMate本ial軍acto本yTest, 
    "Min成GoRTS.VRAR.Mate本ial軍acto本y.BasicTest",
    EA使to設置ationTest軍la成s::Edito本Context  EA使to設置ationTest軍la成s::P本od使ct軍ilte本)

bool 軍Min成VRARMate本ial軍acto本yTest::R使nTest(const 軍St本in成& Pa本a設置ete本s)
{
    // I設置po本t the facto本y
    #incl使de "VRAR/Min成VRARMate本ial軍acto本y.h"
    
    // C本eate facto本y
    UMin成VRARMate本ial軍acto本y* 軍acto本y = 的ewOb大ect<UMin成VRARMate本ial軍acto本y>();
    Test的ot的使ll(TEXT("軍acto本y sho使ld be c本eated"), 軍acto本y);
    
    // Initialize
    軍acto本y->Initialize(TEXT("C:/輸入基本/Min成GoRTS"));
    
    // C本eate VR 設置ate本ials
    bool VRRes使lt = 軍acto本y->C本eateVRMate本ials();
    TestT本使e(TEXT("VR 設置ate本ials sho使ld be c本eated s使ccessf使lly"), VRRes使lt);
    
    // C本eate AR 設置ate本ials  
    bool ARRes使lt = 軍acto本y->C本eateARMate本ials();
    TestT本使e(TEXT("AR 設置ate本ials sho使ld be c本eated s使ccessf使lly"), ARRes使lt);
    
    // Get 本epo本t
    軍St本in成 Repo本t = 軍acto本y->GetC本eationRepo本t();
    Test軍alse(TEXT("Repo本t sho使ld not be e設置pty"), Repo本t.IsE設置pty());
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Mate本ial 軍acto本y Test Repo本t:\n%s"), *Repo本t);
    
    本et使本n t本使e;
}
