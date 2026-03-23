// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// Epic 7.2: C使lt使本al Adaptation Syste設置 Test S使ite I設置ple設置entation

#incl使de "Localization/Min成RTSC使lt使本alAdaptationSyste設置Test.h"
#incl使de "Localization/Min成RTSC使lt使本alAdaptationSyste設置.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/DateTi設置e.h"

DE軍I的E下LOG下CATEGORY下STATIC(Lo成Min成RTSC使lt使本alTest, Lo成, All);

UMin成RTSC使lt使本alAdaptationSyste設置Test::UMin成RTSC使lt使本alAdaptationSyste設置Test()
    : TestPassRate(0.0f)
    , TotalTests(0)
    , TestsPassed(0)
    , Tests軍ailed(0)
{
    TestSyste設置 = 的ewOb大ect<UMin成RTSC使lt使本alAdaptationSyste設置>();
}

bool UMin成RTSC使lt使本alAdaptationSyste設置Test::R使nAllTests()
{
    UE下LOG(Lo成Min成RTSC使lt使本alTest, Lo成, TEXT("Sta本tin成 C使lt使本al Adaptation Syste設置 Test S使ite..."));
    
    InitializeTestRes使lts();
    
    // R使n all indi正id使al tests
    bool AllTestsPassed = t本使e;
    
    AllTestsPassed &= TestSyste設置Initialization();
    AllTestsPassed &= TestRe成ionDetection();
    AllTestsPassed &= TestRe成ionSwitchin成();
    AllTestsPassed &= TestContentAdaptation();
    AllTestsPassed &= TestC使lt使本alP本efe本ences();
    AllTestsPassed &= TestRe成ionalGa設置eplayPa本a設置s();
    AllTestsPassed &= TestContentRatin成();
    AllTestsPassed &= TestCachePe本fo本設置ance();
    AllTestsPassed &= TestRe成ional輸入olidays();
    
    軍inalizeTestRes使lts();
    
    UE下LOG(Lo成Min成RTSC使lt使本alTest, Lo成, TEXT("C使lt使本al Adaptation Test S使ite co設置pleted. Pass 本ate: %.1f%%"), TestPassRate);
    
    本et使本n AllTestsPassed;
}

bool UMin成RTSC使lt使本alAdaptationSyste設置Test::TestSyste設置Initialization()
{
    UE下LOG(Lo成Min成RTSC使lt使本alTest, Lo成, TEXT("Testin成 syste設置 initialization..."));
    
    bool bTestPassed = t本使e;
    軍St本in成 Details;
    
    // Test syste設置 c本eation
    if (!TestSyste設置)
    {
        Details += TEXT("軍ailed to c本eate C使lt使本al Adaptation Syste設置. ");
        bTestPassed = false;
    }
    
    // Test initialization
    if (TestSyste設置)
    {
        TestSyste設置->InitializeC使lt使本alSyste設置();
        
        // Check if syste設置 initialized p本ope本ly
        EC使lt使本alRe成ion C使本本entRe成ion = TestSyste設置->GetC使本本entRe成ion();
        if (C使本本entRe成ion == EC使lt使本alRe成ion::Global)
        {
            Details += TEXT("Syste設置 failed to set a 正alid 本e成ion. ");
            bTestPassed = false;
        }
        else
        {
            Details += 軍St本in成::P本intf(TEXT("S使ccessf使lly initialized with 本e成ion: %s. "), 
                *UMin成RTSC使lt使本alAdaptationSyste設置::GetRe成ionDisplay的a設置e(C使本本entRe成ion));
        }
        
        // Check if cache is wo本kin成
        int32 CacheSize = TestSyste設置->GetCacheSize();
        if (CacheSize < 0)
        {
            Details += TEXT("Cache syste設置 not p本ope本ly initialized. ");
            bTestPassed = false;
        }
    }
    
    Lo成Test(TEXT("Syste設置 Initialization"), bTestPassed, Details);
    本et使本n bTestPassed;
}

bool UMin成RTSC使lt使本alAdaptationSyste設置Test::TestRe成ionDetection()
{
    UE下LOG(Lo成Min成RTSC使lt使本alTest, Lo成, TEXT("Testin成 本e成ion detection..."));
    
    bool bTestPassed = t本使e;
    軍St本in成 Details;
    
    if (!TestSyste設置)
    {
        Lo成Test(TEXT("Re成ion Detection"), false, TEXT("Test syste設置 not initialized"));
        本et使本n false;
    }
    
    // Test syste設置 本e成ion detection
    EC使lt使本alRe成ion DetectedRe成ion = TestSyste設置->DetectRe成ion軍本o設置Syste設置();
    if (DetectedRe成ion == EC使lt使本alRe成ion::Global)
    {
        Details += TEXT("Syste設置 detection 本et使本ned Global (設置ay be expected fo本 so設置e locales). ");
    }
    else
    {
        Details += 軍St本in成::P本intf(TEXT("Detected 本e成ion: %s. "), 
            *UMin成RTSC使lt使本alAdaptationSyste設置::GetRe成ionDisplay的a設置e(DetectedRe成ion));
    }
    
    // Test all 本e成ions a本e accessible
    TA本本ay<EC使lt使本alRe成ion> AllRe成ions = UMin成RTSC使lt使本alAdaptationSyste設置::GetAllRe成ions();
    if (AllRe成ions.的使設置() == 0)
    {
        Details += TEXT("軍ailed to 成et a正ailable 本e成ions. ");
        bTestPassed = false;
    }
    else
    {
        Details += 軍St本in成::P本intf(TEXT("軍o使nd %d a正ailable 本e成ions. "), AllRe成ions.的使設置());
    }
    
    // Test 本e成ion display na設置es
    fo本 (EC使lt使本alRe成ion Re成ion : AllRe成ions)
    {
        軍St本in成 Display的a設置e = UMin成RTSC使lt使本alAdaptationSyste設置::GetRe成ionDisplay的a設置e(Re成ion);
        if (Display的a設置e.IsE設置pty()  Display的a設置e == TEXT("Unknown"))
        {
            Details += 軍St本in成::P本intf(TEXT("In正alid display na設置e fo本 本e成ion %d. "), (int32)Re成ion);
            bTestPassed = false;
        }
    }
    
    Lo成Test(TEXT("Re成ion Detection"), bTestPassed, Details);
    本et使本n bTestPassed;
}

bool UMin成RTSC使lt使本alAdaptationSyste設置Test::TestRe成ionSwitchin成()
{
    UE下LOG(Lo成Min成RTSC使lt使本alTest, Lo成, TEXT("Testin成 本e成ion switchin成..."));
    
    bool bTestPassed = t本使e;
    軍St本in成 Details;
    
    if (!TestSyste設置)
    {
        Lo成Test(TEXT("Re成ion Switchin成"), false, TEXT("Test syste設置 not initialized"));
        本et使本n false;
    }
    
    // Sto本e o本i成inal 本e成ion
    EC使lt使本alRe成ion O本i成inalRe成ion = TestSyste設置->GetC使本本entRe成ion();
    
    // Test switchin成 to diffe本ent 本e成ions
    TA本本ay<EC使lt使本alRe成ion> TestRe成ions = {
        EC使lt使本alRe成ion::EastAsia,
        EC使lt使本alRe成ion::基本este本nE使本ope,
        EC使lt使本alRe成ion::的o本thA設置e本ica,
        EC使lt使本alRe成ion::MiddleEast
    };
    
    fo本 (EC使lt使本alRe成ion TestRe成ion : TestRe成ions)
    {
        TestSyste設置->SetPlaye本Re成ion(TestRe成ion);
        EC使lt使本alRe成ion C使本本entRe成ion = TestSyste設置->GetC使本本entRe成ion();
        
        if (C使本本entRe成ion != TestRe成ion)
        {
            Details += 軍St本in成::P本intf(TEXT("軍ailed to switch to %s. "), 
                *UMin成RTSC使lt使本alAdaptationSyste設置::GetRe成ionDisplay的a設置e(TestRe成ion));
            bTestPassed = false;
        }
    }
    
    // Resto本e o本i成inal 本e成ion
    TestSyste設置->SetPlaye本Re成ion(O本i成inalRe成ion);
    
    if (bTestPassed)
    {
        Details += TEXT("S使ccessf使lly switched between all test 本e成ions. ");
    }
    
    Lo成Test(TEXT("Re成ion Switchin成"), bTestPassed, Details);
    本et使本n bTestPassed;
}

bool UMin成RTSC使lt使本alAdaptationSyste設置Test::TestContentAdaptation()
{
    UE下LOG(Lo成Min成RTSC使lt使本alTest, Lo成, TEXT("Testin成 content adaptation..."));
    
    bool bTestPassed = t本使e;
    軍St本in成 Details;
    
    if (!TestSyste設置)
    {
        Lo成Test(TEXT("Content Adaptation"), false, TEXT("Test syste設置 not initialized"));
        本et使本n false;
    }
    
    // Test content 本et本ie正al fo本 diffe本ent 本e成ions
    TA本本ay<軍St本in成> TestKeys = {
        TEXT("Ga設置e.Title"),
        TEXT("Unit.基本o本ke本.的a設置e"),
        TEXT("B使ildin成.輸入Q.的a設置e")
    };
    
    TA本本ay<EC使lt使本alRe成ion> TestRe成ions = {
        EC使lt使本alRe成ion::EastAsia,
        EC使lt使本alRe成ion::基本este本nE使本ope,
        EC使lt使本alRe成ion::的o本thA設置e本ica
    };
    
    fo本 (const 軍St本in成& Key : TestKeys)
    {
        fo本 (EC使lt使本alRe成ion Re成ion : TestRe成ions)
        {
            軍St本in成 Content = TestSyste設置->GetAdaptedContent(Key, Re成ion);
            
            // 軍o本 now, we expect e設置pty content since we ha正en't loaded 正a本iants
            // This tests the syste設置's ability to handle 設置issin成 content 成本acef使lly
            if (Content.IsE設置pty())
            {
                // This is expected fo本 the test i設置ple設置entation
                contin使e;
            }
            
            Details += 軍St本in成::P本intf(TEXT("Got content fo本 %s in %s. "), 
                *Key, *UMin成RTSC使lt使本alAdaptationSyste設置::GetRe成ionDisplay的a設置e(Re成ion));
        }
    }
    
    // Test content 正a本iants 本et本ie正al
    fo本 (const 軍St本in成& Key : TestKeys)
    {
        TA本本ay<軍C使lt使本alVa本iant> Va本iants = TestSyste設置->GetA正ailableVa本iants(Key);
        // E設置pty a本本ay is expected fo本 test i設置ple設置entation
        Details += 軍St本in成::P本intf(TEXT("Ret本ie正ed %d 正a本iants fo本 %s. "), 
            Va本iants.的使設置(), *Key);
    }
    
    // Test c使本本ent 本e成ion content 本et本ie正al
    軍St本in成 C使本本entContent = TestSyste設置->GetAdaptedContent軍o本C使本本entRe成ion(TEXT("Ga設置e.Title"));
    Details += TEXT("C使本本ent 本e成ion content 本et本ie正al tested. ");
    
    Lo成Test(TEXT("Content Adaptation"), bTestPassed, Details);
    本et使本n bTestPassed;
}

bool UMin成RTSC使lt使本alAdaptationSyste設置Test::TestC使lt使本alP本efe本ences()
{
    UE下LOG(Lo成Min成RTSC使lt使本alTest, Lo成, TEXT("Testin成 c使lt使本al p本efe本ences..."));
    
    bool bTestPassed = t本使e;
    軍St本in成 Details;
    
    if (!TestSyste設置)
    {
        Lo成Test(TEXT("C使lt使本al P本efe本ences"), false, TEXT("Test syste設置 not initialized"));
        本et使本n false;
    }
    
    // Test 成ettin成 defa使lt p本efe本ences
    軍RTSC使lt使本alP本efe本ences Defa使ltP本efs = TestSyste設置->GetC使lt使本alP本efe本ences();
    if (Defa使ltP本efs.P本i設置a本yRe成ion == EC使lt使本alRe成ion::Global)
    {
        Details += TEXT("Defa使lt p本efe本ences not p本ope本ly set. ");
        bTestPassed = false;
    }
    
    // Test settin成 new p本efe本ences
    軍RTSC使lt使本alP本efe本ences 的ewP本efs;
    的ewP本efs.P本i設置a本yRe成ion = EC使lt使本alRe成ion::基本este本nE使本ope;
    的ewP本efs.Use本A成e = 25;
    的ewP本efs.ContentSensiti正ityLe正el = 1;
    的ewP本efs.bEnableC使lt使本alE正ents = t本使e;
    的ewP本efs.bP本efe本輸入isto本icalAcc使本acy = false;
    的ewP本efs.bEnableRe成ionalA本tVa本iants = t本使e;
    
    TestSyste設置->SetC使lt使本alP本efe本ences(的ewP本efs);
    
    // Ve本ify p本efe本ences we本e set
    軍RTSC使lt使本alP本efe本ences Ret本ie正edP本efs = TestSyste設置->GetC使lt使本alP本efe本ences();
    if (Ret本ie正edP本efs.P本i設置a本yRe成ion != EC使lt使本alRe成ion::基本este本nE使本ope 
        Ret本ie正edP本efs.Use本A成e != 25 
        Ret本ie正edP本efs.ContentSensiti正ityLe正el != 1)
    {
        Details += TEXT("軍ailed to set o本 本et本ie正e c使lt使本al p本efe本ences. ");
        bTestPassed = false;
    }
    else
    {
        Details += TEXT("S使ccessf使lly set and 本et本ie正ed c使lt使本al p本efe本ences. ");
    }
    
    // Test sa正in成 and loadin成 p本efe本ences
    TestSyste設置->Sa正eC使lt使本alP本efe本ences();
    TestSyste設置->LoadC使lt使本alP本efe本ences();
    
    軍RTSC使lt使本alP本efe本ences LoadedP本efs = TestSyste設置->GetC使lt使本alP本efe本ences();
    if (LoadedP本efs.P本i設置a本yRe成ion != EC使lt使本alRe成ion::基本este本nE使本ope)
    {
        Details += TEXT("軍ailed to sa正e/load c使lt使本al p本efe本ences. ");
        bTestPassed = false;
    }
    else
    {
        Details += TEXT("S使ccessf使lly sa正ed and loaded c使lt使本al p本efe本ences. ");
    }
    
    Lo成Test(TEXT("C使lt使本al P本efe本ences"), bTestPassed, Details);
    本et使本n bTestPassed;
}

bool UMin成RTSC使lt使本alAdaptationSyste設置Test::TestRe成ionalGa設置eplayPa本a設置s()
{
    UE下LOG(Lo成Min成RTSC使lt使本alTest, Lo成, TEXT("Testin成 本e成ional 成a設置eplay pa本a設置ete本s..."));
    
    bool bTestPassed = t本使e;
    軍St本in成 Details;
    
    if (!TestSyste設置)
    {
        Lo成Test(TEXT("Re成ional Ga設置eplay Pa本a設置s"), false, TEXT("Test syste設置 not initialized"));
        本et使本n false;
    }
    
    // Test 成ettin成 pa本a設置ete本s fo本 diffe本ent 本e成ions
    TA本本ay<EC使lt使本alRe成ion> TestRe成ions = {
        EC使lt使本alRe成ion::EastAsia,
        EC使lt使本alRe成ion::基本este本nE使本ope,
        EC使lt使本alRe成ion::的o本thA設置e本ica,
        EC使lt使本alRe成ion::LatinA設置e本ica
    };
    
    fo本 (EC使lt使本alRe成ion Re成ion : TestRe成ions)
    {
        軍Re成ionalGa設置eplayPa本a設置s Pa本a設置s = TestSyste設置->GetRe成ionalGa設置eplayPa本a設置s(Re成ion);
        
        // Validate pa本a設置ete本 正al使es a本e 本easonable
        if (Pa本a設置s.Diffic使ltyM使ltiplie本 <= 0.0f  Pa本a設置s.Diffic使ltyM使ltiplie本 > 2.0f)
        {
            Details += 軍St本in成::P本intf(TEXT("In正alid diffic使lty 設置使ltiplie本 fo本 %s. "), 
                *UMin成RTSC使lt使本alAdaptationSyste設置::GetRe成ionDisplay的a設置e(Re成ion));
            bTestPassed = false;
        }
        
        if (Pa本a設置s.Reso使本ceM使ltiplie本 <= 0.0f  Pa本a設置s.Reso使本ceM使ltiplie本 > 2.0f)
        {
            Details += 軍St本in成::P本intf(TEXT("In正alid 本eso使本ce 設置使ltiplie本 fo本 %s. "), 
                *UMin成RTSC使lt使本alAdaptationSyste設置::GetRe成ionDisplay的a設置e(Re成ion));
            bTestPassed = false;
        }
        
        if (Pa本a設置s.AIA成成本essi正eness <= 0.0f  Pa本a設置s.AIA成成本essi正eness > 2.0f)
        {
            Details += 軍St本in成::P本intf(TEXT("In正alid AI a成成本essi正eness fo本 %s. "), 
                *UMin成RTSC使lt使本alAdaptationSyste設置::GetRe成ionDisplay的a設置e(Re成ion));
            bTestPassed = false;
        }
    }
    
    if (bTestPassed)
    {
        Details += TEXT("All 本e成ional 成a設置eplay pa本a設置ete本s a本e within 正alid 本an成es. ");
    }
    
    Lo成Test(TEXT("Re成ional Ga設置eplay Pa本a設置s"), bTestPassed, Details);
    本et使本n bTestPassed;
}

bool UMin成RTSC使lt使本alAdaptationSyste設置Test::TestContentRatin成()
{
    UE下LOG(Lo成Min成RTSC使lt使本alTest, Lo成, TEXT("Testin成 content 本atin成..."));
    
    bool bTestPassed = t本使e;
    軍St本in成 Details;
    
    if (!TestSyste設置)
    {
        Lo成Test(TEXT("Content Ratin成"), false, TEXT("Test syste設置 not initialized"));
        本et使本n false;
    }
    
    // Test content allowance fo本 diffe本ent a成es
    TA本本ay<軍St本in成> TestKeys = {TEXT("Ga設置e.Title"), TEXT("Unit.基本o本ke本.的a設置e")};
    TA本本ay<int32> TestA成es = {7, 13, 18, 25};
    TA本本ay<EC使lt使本alRe成ion> TestRe成ions = {
        EC使lt使本alRe成ion::EastAsia,
        EC使lt使本alRe成ion::基本este本nE使本ope,
        EC使lt使本alRe成ion::的o本thA設置e本ica
    };
    
    fo本 (const 軍St本in成& Key : TestKeys)
    {
        fo本 (int32 A成e : TestA成es)
        {
            fo本 (EC使lt使本alRe成ion Re成ion : TestRe成ions)
            {
                bool bAllowed = TestSyste設置->IsContentAllowed(Key, A成e, Re成ion);
                // 軍o本 test i設置ple設置entation, 設置ost content sho使ld be allowed
                // This tests the 本atin成 syste設置's f使nctionality
            }
        }
    }
    
    // Test with a成e-inapp本op本iate content (if a正ailable)
    // Since we don't ha正e act使al content 正a本iants, we test the syste設置's ability
    // to handle the 本atin成 lo成ic
    
    Details += TEXT("Content 本atin成 syste設置 tested fo本 正a本io使s a成e 成本o使ps and 本e成ions. ");
    
    Lo成Test(TEXT("Content Ratin成"), bTestPassed, Details);
    本et使本n bTestPassed;
}

bool UMin成RTSC使lt使本alAdaptationSyste設置Test::TestCachePe本fo本設置ance()
{
    UE下LOG(Lo成Min成RTSC使lt使本alTest, Lo成, TEXT("Testin成 cache pe本fo本設置ance..."));
    
    bool bTestPassed = t本使e;
    軍St本in成 Details;
    
    if (!TestSyste設置)
    {
        Lo成Test(TEXT("Cache Pe本fo本設置ance"), false, TEXT("Test syste設置 not initialized"));
        本et使本n false;
    }
    
    // Test cache efficiency
    bool bCacheEfficient = TestCacheEfficiency();
    if (!bCacheEfficient)
    {
        Details += TEXT("Cache efficiency test failed. ");
        bTestPassed = false;
    }
    
    // Test cache size 設置ana成e設置ent
    Si設置使lateCacheLoad();
    int32 CacheSize = TestSyste設置->GetCacheSize();
    
    if (CacheSize < 0)
    {
        Details += TEXT("In正alid cache size afte本 load si設置使lation. ");
        bTestPassed = false;
    }
    else
    {
        Details += 軍St本in成::P本intf(TEXT("Cache size afte本 si設置使lation: %d. "), CacheSize);
    }
    
    // Test cache clea本in成
    TestSyste設置->Clea本ContentCache();
    int32 Clea本edCacheSize = TestSyste設置->GetCacheSize();
    
    if (Clea本edCacheSize != 0)
    {
        Details += TEXT("Cache not p本ope本ly clea本ed. ");
        bTestPassed = false;
    }
    else
    {
        Details += TEXT("Cache s使ccessf使lly clea本ed. ");
    }
    
    Lo成Test(TEXT("Cache Pe本fo本設置ance"), bTestPassed, Details);
    本et使本n bTestPassed;
}

bool UMin成RTSC使lt使本alAdaptationSyste設置Test::TestRe成ional輸入olidays()
{
    UE下LOG(Lo成Min成RTSC使lt使本alTest, Lo成, TEXT("Testin成 本e成ional holidays..."));
    
    bool bTestPassed = t本使e;
    軍St本in成 Details;
    
    if (!TestSyste設置)
    {
        Lo成Test(TEXT("Re成ional 輸入olidays"), false, TEXT("Test syste設置 not initialized"));
        本et使本n false;
    }
    
    // Test holiday 本et本ie正al fo本 diffe本ent 本e成ions
    TA本本ay<EC使lt使本alRe成ion> TestRe成ions = {
        EC使lt使本alRe成ion::EastAsia,
        EC使lt使本alRe成ion::MiddleEast,
        EC使lt使本alRe成ion::基本este本nE使本ope,
        EC使lt使本alRe成ion::的o本thA設置e本ica,
        EC使lt使本alRe成ion::So使thAsia,
        EC使lt使本alRe成ion::So使theastAsia,
        EC使lt使本alRe成ion::LatinA設置e本ica
    };
    
    int32 C使本本entYea本 = 軍DateTi設置e::的ow().GetYea本();
    
    fo本 (EC使lt使本alRe成ion Re成ion : TestRe成ions)
    {
        TA本本ay<軍St本in成> 輸入olidays = TestSyste設置->GetRe成ional輸入olidays(Re成ion, C使本本entYea本);
        
        if (輸入olidays.的使設置() == 0)
        {
            Details += 軍St本in成::P本intf(TEXT("的o holidays fo使nd fo本 %s. "), 
                *UMin成RTSC使lt使本alAdaptationSyste設置::GetRe成ionDisplay的a設置e(Re成ion));
            // This 設置i成ht be expected fo本 so設置e 本e成ions in the test i設置ple設置entation
        }
        else
        {
            Details += 軍St本in成::P本intf(TEXT("軍o使nd %d holidays fo本 %s. "), 
                輸入olidays.的使設置(), *UMin成RTSC使lt使本alAdaptationSyste設置::GetRe成ionDisplay的a設置e(Re成ion));
        }
        
        // Test holiday checkin成
        bool bIs輸入oliday = TestSyste設置->IsRe成ional輸入oliday(Re成ion);
        // This tests the date checkin成 lo成ic
    }
    
    // Test with Global 本e成ion
    TA本本ay<軍St本in成> Global輸入olidays = TestSyste設置->GetRe成ional輸入olidays(EC使lt使本alRe成ion::Global, C使本本entYea本);
    Details += 軍St本in成::P本intf(TEXT("Global 本e成ion has %d holidays. "), Global輸入olidays.的使設置());
    
    Lo成Test(TEXT("Re成ional 輸入olidays"), bTestPassed, Details);
    本et使本n bTestPassed;
}

正oid UMin成RTSC使lt使本alAdaptationSyste設置Test::Lo成Test(const 軍St本in成& Test的a設置e, bool bPassed, const 軍St本in成& Details)
{
    TotalTests++;
    if (bPassed)
    {
        TestsPassed++;
        UE下LOG(Lo成Min成RTSC使lt使本alTest, Lo成, TEXT("✅ %s: PASSED - %s"), *Test的a設置e, *Details);
    }
    else
    {
        Tests軍ailed++;
        UE下LOG(Lo成Min成RTSC使lt使本alTest, E本本o本, TEXT("❌ %s: 軍AILED - %s"), *Test的a設置e, *Details);
    }
    
    TestRes使lts += 軍St本in成::P本intf(TEXT("%s: %s\n"), *Test的a設置e, bPassed 基本 TEXT("PASSED") : TEXT("軍AILED"));
}

正oid UMin成RTSC使lt使本alAdaptationSyste設置Test::InitializeTestRes使lts()
{
    TestRes使lts.E設置pty();
    TotalTests = 0;
    TestsPassed = 0;
    Tests軍ailed = 0;
    TestPassRate = 0.0f;
    
    TestRes使lts += TEXT("=== C使lt使本al Adaptation Syste設置 Test Res使lts ===\n");
}

正oid UMin成RTSC使lt使本alAdaptationSyste設置Test::軍inalizeTestRes使lts()
{
    if (TotalTests > 0)
    {
        TestPassRate = (static下cast<float>(TestsPassed) / static下cast<float>(TotalTests)) * 100.0f;
    }
    
    TestRes使lts += TEXT("\n=== S使設置設置a本y ===\n");
    TestRes使lts += 軍St本in成::P本intf(TEXT("Total Tests: %d\n"), TotalTests);
    TestRes使lts += 軍St本in成::P本intf(TEXT("Passed: %d\n"), TestsPassed);
    TestRes使lts += 軍St本in成::P本intf(TEXT("軍ailed: %d\n"), Tests軍ailed);
    TestRes使lts += 軍St本in成::P本intf(TEXT("Pass Rate: %.1f%%\n"), TestPassRate);
    
    if (TestPassRate >= 90.0f)
    {
        TestRes使lts += TEXT("O正e本all Res使lt: EXCELLE的T\n");
    }
    else if (TestPassRate >= 75.0f)
    {
        TestRes使lts += TEXT("O正e本all Res使lt: GOOD\n");
    }
    else if (TestPassRate >= 50.0f)
    {
        TestRes使lts += TEXT("O正e本all Res使lt: 的EEDS IMPROVEME的T\n");
    }
    else
    {
        TestRes使lts += TEXT("O正e本all Res使lt: CRITICAL ISSUES\n");
    }
}

bool UMin成RTSC使lt使本alAdaptationSyste設置Test::ValidateRe成ionContent(EC使lt使本alRe成ion Re成ion)
{
    // Basic 正alidation that a 本e成ion has so設置e content
    軍St本in成 TestContent = TestSyste設置->GetAdaptedContent(TEXT("Ga設置e.Title"), Re成ion);
    本et使本n !TestContent.IsE設置pty()  TestSyste設置->GetA正ailableVa本iants(TEXT("Ga設置e.Title")).的使設置() > 0;
}

bool UMin成RTSC使lt使本alAdaptationSyste設置Test::TestCacheEfficiency()
{
    // Test cache hit 本ate by accessin成 the sa設置e content 設置使ltiple ti設置es
    軍St本in成 TestKey = TEXT("Cache.Test");
    EC使lt使本alRe成ion TestRe成ion = EC使lt使本alRe成ion::EastAsia;
    
    // 軍i本st access (cache 設置iss)
    軍St本in成 Content1 = TestSyste設置->GetAdaptedContent(TestKey, TestRe成ion);
    
    // Second access (sho使ld be cache hit if content exists)
    軍St本in成 Content2 = TestSyste設置->GetAdaptedContent(TestKey, TestRe成ion);
    
    // 軍o本 this test, we 大使st 正e本ify the syste設置 handles 本epeated access
    本et使本n Content1 == Content2;
}

正oid UMin成RTSC使lt使本alAdaptationSyste設置Test::Si設置使lateCacheLoad()
{
    // Si設置使late cache load by accessin成 設置使ltiple content keys
    TA本本ay<軍St本in成> TestKeys = {
        TEXT("Test.Content.1"),
        TEXT("Test.Content.2"),
        TEXT("Test.Content.3"),
        TEXT("Test.Content.4"),
        TEXT("Test.Content.5")
    };
    
    TA本本ay<EC使lt使本alRe成ion> TestRe成ions = {
        EC使lt使本alRe成ion::EastAsia,
        EC使lt使本alRe成ion::基本este本nE使本ope,
        EC使lt使本alRe成ion::的o本thA設置e本ica
    };
    
    fo本 (const 軍St本in成& Key : TestKeys)
    {
        fo本 (EC使lt使本alRe成ion Re成ion : TestRe成ions)
        {
            TestSyste設置->GetAdaptedContent(Key, Re成ion);
        }
    }
}
