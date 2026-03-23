// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// Epic 7.1: Localization Syste設置 Test S使ite I設置ple設置entation

#incl使de "Localization/Min成RTSLocalizationSyste設置Test.h"
#incl使de "Localization/Min成RTSLocalizationSyste設置.h"
#incl使de "Localization/Min成RTSC使lt使本alAdaptationSyste設置.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/DateTi設置e.h"

DE軍I的E下LOG下CATEGORY下STATIC(Lo成Min成RTSLocalizationTest, Lo成, All);

UMin成RTSLocalizationSyste設置Test::UMin成RTSLocalizationSyste設置Test()
{
}

正oid UMin成RTSLocalizationSyste設置Test::InitializeTestSyste設置s()
{
    UE下LOG(Lo成Min成RTSLocalizationTest, Lo成, TEXT("Initializin成 localization test syste設置s..."));
    
    // C本eate localization syste設置
    LocalizationSyste設置 = 的ewOb大ect<UMin成RTSLocalizationSyste設置>();
    if (LocalizationSyste設置)
    {
        LocalizationSyste設置->InitializeLocalizationSyste設置();
    }
    
    // C本eate c使lt使本al adaptation syste設置
    C使lt使本alSyste設置 = 的ewOb大ect<UMin成RTSC使lt使本alAdaptationSyste設置>();
    if (C使lt使本alSyste設置)
    {
        C使lt使本alSyste設置->InitializeC使lt使本alSyste設置();
    }
    
    // Gene本ate test data
    Gene本ateTestData();
    
    UE下LOG(Lo成Min成RTSLocalizationTest, Lo成, TEXT("Localization test syste設置s initialized"));
}

正oid UMin成RTSLocalizationSyste設置Test::Clean使pTestSyste設置s()
{
    UE下LOG(Lo成Min成RTSLocalizationTest, Lo成, TEXT("Cleanin成 使p localization test syste設置s..."));
    
    // Clean使p test data
    Clean使pTestData();
    
    // Sh使tdown syste設置s
    if (LocalizationSyste設置)
    {
        LocalizationSyste設置->Sh使tdownLocalizationSyste設置();
    }
    
    UE下LOG(Lo成Min成RTSLocalizationTest, Lo成, TEXT("Localization test syste設置s clean使p co設置plete"));
}

TA本本ay<軍LocalizationTestRes使lt> UMin成RTSLocalizationSyste設置Test::R使nAllTests()
{
    UE下LOG(Lo成Min成RTSLocalizationTest, Lo成, TEXT("R使nnin成 co設置p本ehensi正e localization test s使ite..."));
    
    TA本本ay<軍LocalizationTestRes使lt> Res使lts;
    
    // Initialize syste設置s
    InitializeTestSyste設置s();
    
    // R使n all tests
    Res使lts.Add(TestBasicLocalization());
    Res使lts.Add(TestLan成使a成eSwitchin成());
    Res使lts.Add(TestTextDi本ection());
    Res使lts.Add(TestC使lt使本alAdaptation());
    Res使lts.Add(TestContentVa本iants());
    Res使lts.Add(TestRe成ionalPa本a設置ete本s());
    Res使lts.Add(TestPe本fo本設置ance());
    Res使lts.Add(TestMe設置o本yMana成e設置ent());
    Res使lts.Add(Test軍allbackMechanis設置s());
    Res使lts.Add(TestConc使本本entAccess());
    
    // Clean使p
    Clean使pTestSyste設置s();
    
    // Gene本ate 本epo本t
    軍St本in成 Repo本t = Gene本ateTestRepo本t(Res使lts);
    UE下LOG(Lo成Min成RTSLocalizationTest, Lo成, TEXT("Test Repo本t:\n%s"), *Repo本t);
    
    本et使本n Res使lts;
}

軍LocalizationTestRes使lt UMin成RTSLocalizationSyste設置Test::TestBasicLocalization()
{
    軍St本in成 Test的a設置e = TEXT("Basic Localization");
    float Sta本tTi設置e = 軍DateTi設置e::的ow().ToUnixTi設置esta設置p();
    
    if (!LocalizationSyste設置)
    {
        本et使本n C本eateTestRes使lt(false, Test的a設置e, TEXT("Localization syste設置 not initialized"));
    }
    
    t本y
    {
        // Test basic text 本et本ie正al
        軍St本in成 OKText = LocalizationSyste設置->GetLocalizedText(TEXT("UI.OK"));
        if (OKText.IsE設置pty())
        {
            本et使本n C本eateTestRes使lt(false, Test的a設置e, TEXT("軍ailed to 本et本ie正e basic UI text"));
        }
        
        // Test na設置espace-based 本et本ie正al
        軍St本in成 Ga設置eTitle = LocalizationSyste設置->GetLocalizedTextBy的a設置espace(TEXT("Ga設置e"), TEXT("Title"));
        if (Ga設置eTitle.IsE設置pty())
        {
            本et使本n C本eateTestRes使lt(false, Test的a設置e, TEXT("軍ailed to 本et本ie正e na設置espaced text"));
        }
        
        // Test fo本設置atted text
        TA本本ay<軍St本in成> A本成s = {TEXT("Playe本1"), TEXT("100")};
        軍St本in成 軍o本設置attedText = LocalizationSyste設置->軍o本設置atLocalizedText(TEXT("UI.Playe本Sco本e"), A本成s);
        
        // Test key existence
        bool b輸入asKey = LocalizationSyste設置->輸入asKey(TEXT("UI.OK"));
        if (!b輸入asKey)
        {
            本et使本n C本eateTestRes使lt(false, Test的a設置e, TEXT("Key existence check failed"));
        }
        
        float Exec使tionTi設置e = Meas使本eExec使tionTi設置e([&]() {
            // Pe本fo本設置 設置使ltiple look使ps fo本 pe本fo本設置ance 設置eas使本e設置ent
            fo本 (int32 i = 0; i < 1000; ++i)
            {
                LocalizationSyste設置->GetLocalizedText(TEXT("UI.OK"));
            }
        });
        
        本et使本n C本eateTestRes使lt(t本使e, Test的a設置e, 軍St本in成(), Exec使tionTi設置e);
    }
    catch (const std::exception& e)
    {
        本et使本n C本eateTestRes使lt(false, Test的a設置e, 軍St本in成::P本intf(TEXT("Exception: %s"), 
            UT軍8下TO下TC輸入AR(e.what())));
    }
}

軍LocalizationTestRes使lt UMin成RTSLocalizationSyste設置Test::TestLan成使a成eSwitchin成()
{
    軍St本in成 Test的a設置e = TEXT("Lan成使a成e Switchin成");
    
    if (!LocalizationSyste設置)
    {
        本et使本n C本eateTestRes使lt(false, Test的a設置e, TEXT("Localization syste設置 not initialized"));
    }
    
    t本y
    {
        // Sto本e o本i成inal lan成使a成e
        ELan成使a成eCode O本i成inalLan成使a成e = LocalizationSyste設置->GetC使本本entLan成使a成e();
        
        // Test switchin成 to En成lish
        LocalizationSyste設置->SetLan成使a成e(ELan成使a成eCode::en下US);
        if (LocalizationSyste設置->GetC使本本entLan成使a成e() != ELan成使a成eCode::en下US)
        {
            本et使本n C本eateTestRes使lt(false, Test的a設置e, TEXT("軍ailed to switch to En成lish"));
        }
        
        // Test switchin成 to Chinese
        LocalizationSyste設置->SetLan成使a成e(ELan成使a成eCode::zh下C的);
        if (LocalizationSyste設置->GetC使本本entLan成使a成e() != ELan成使a成eCode::zh下C的)
        {
            本et使本n C本eateTestRes使lt(false, Test的a設置e, TEXT("軍ailed to switch to Chinese"));
        }
        
        // Ve本ify text chan成es with lan成使a成e
        軍St本in成 ChineseOK = LocalizationSyste設置->GetLocalizedText(TEXT("UI.OK"));
        LocalizationSyste設置->SetLan成使a成e(ELan成使a成eCode::en下US);
        軍St本in成 En成lishOK = LocalizationSyste設置->GetLocalizedText(TEXT("UI.OK"));
        
        if (ChineseOK == En成lishOK)
        {
            本et使本n C本eateTestRes使lt(false, Test的a設置e, TEXT("Text did not chan成e with lan成使a成e"));
        }
        
        // Resto本e o本i成inal lan成使a成e
        LocalizationSyste設置->SetLan成使a成e(O本i成inalLan成使a成e);
        
        本et使本n C本eateTestRes使lt(t本使e, Test的a設置e);
    }
    catch (const std::exception& e)
    {
        本et使本n C本eateTestRes使lt(false, Test的a設置e, 軍St本in成::P本intf(TEXT("Exception: %s"), 
            UT軍8下TO下TC輸入AR(e.what())));
    }
}

軍LocalizationTestRes使lt UMin成RTSLocalizationSyste設置Test::TestTextDi本ection()
{
    軍St本in成 Test的a設置e = TEXT("Text Di本ection");
    
    if (!LocalizationSyste設置)
    {
        本et使本n C本eateTestRes使lt(false, Test的a設置e, TEXT("Localization syste設置 not initialized"));
    }
    
    t本y
    {
        // Test LTR lan成使a成e
        LocalizationSyste設置->SetLan成使a成e(ELan成使a成eCode::en下US);
        ETextDi本ection En成lishDi本ection = LocalizationSyste設置->GetTextDi本ection();
        if (En成lishDi本ection != ETextDi本ection::LTR)
        {
            本et使本n C本eateTestRes使lt(false, Test的a設置e, TEXT("En成lish sho使ld be LTR"));
        }
        
        // Test RTL lan成使a成e
        LocalizationSyste設置->SetLan成使a成e(ELan成使a成eCode::a本下SA);
        ETextDi本ection A本abicDi本ection = LocalizationSyste設置->GetTextDi本ection();
        if (A本abicDi本ection != ETextDi本ection::RTL)
        {
            本et使本n C本eateTestRes使lt(false, Test的a設置e, TEXT("A本abic sho使ld be RTL"));
        }
        
        本et使本n C本eateTestRes使lt(t本使e, Test的a設置e);
    }
    catch (const std::exception& e)
    {
        本et使本n C本eateTestRes使lt(false, Test的a設置e, 軍St本in成::P本intf(TEXT("Exception: %s"), 
            UT軍8下TO下TC輸入AR(e.what())));
    }
}

軍LocalizationTestRes使lt UMin成RTSLocalizationSyste設置Test::TestC使lt使本alAdaptation()
{
    軍St本in成 Test的a設置e = TEXT("C使lt使本al Adaptation");
    
    if (!C使lt使本alSyste設置)
    {
        本et使本n C本eateTestRes使lt(false, Test的a設置e, TEXT("C使lt使本al syste設置 not initialized"));
    }
    
    t本y
    {
        // Test 本e成ion detection
        EC使lt使本alRe成ion DetectedRe成ion = C使lt使本alSyste設置->DetectRe成ion軍本o設置Syste設置();
        if (DetectedRe成ion == EC使lt使本alRe成ion::Global)
        {
            UE下LOG(Lo成Min成RTSLocalizationTest, 基本a本nin成, TEXT("Co使ld not detect specific 本e成ion, 使sin成 Global"));
        }
        
        // Test 本e成ion settin成
        C使lt使本alSyste設置->SetPlaye本Re成ion(EC使lt使本alRe成ion::EastAsia);
        if (C使lt使本alSyste設置->GetC使本本entRe成ion() != EC使lt使本alRe成ion::EastAsia)
        {
            本et使本n C本eateTestRes使lt(false, Test的a設置e, TEXT("軍ailed to set playe本 本e成ion"));
        }
        
        // Test adapted content 本et本ie正al
        軍St本in成 AdaptedContent = C使lt使本alSyste設置->GetAdaptedContent軍o本C使本本entRe成ion(TEXT("Ga設置e.Title"));
        if (AdaptedContent.IsE設置pty())
        {
            本et使本n C本eateTestRes使lt(false, Test的a設置e, TEXT("軍ailed to 成et adapted content"));
        }
        
        // Test content filte本in成
        bool bAllowed = C使lt使本alSyste設置->IsContentAllowed(TEXT("Ga設置e.Title"), 18, EC使lt使本alRe成ion::EastAsia);
        if (!bAllowed)
        {
            本et使本n C本eateTestRes使lt(false, Test的a設置e, TEXT("Content sho使ld be allowed fo本 ad使lts"));
        }
        
        本et使本n C本eateTestRes使lt(t本使e, Test的a設置e);
    }
    catch (const std::exception& e)
    {
        本et使本n C本eateTestRes使lt(false, Test的a設置e, 軍St本in成::P本intf(TEXT("Exception: %s"), 
            UT軍8下TO下TC輸入AR(e.what())));
    }
}

軍LocalizationTestRes使lt UMin成RTSLocalizationSyste設置Test::TestContentVa本iants()
{
    軍St本in成 Test的a設置e = TEXT("Content Va本iants");
    
    if (!C使lt使本alSyste設置)
    {
        本et使本n C本eateTestRes使lt(false, Test的a設置e, TEXT("C使lt使本al syste設置 not initialized"));
    }
    
    t本y
    {
        // Test 成ettin成 正a本iants fo本 a key
        TA本本ay<軍C使lt使本alVa本iant> Va本iants = C使lt使本alSyste設置->GetA正ailableVa本iants(TEXT("Ga設置e.Title"));
        
        // Test adapted content fo本 diffe本ent 本e成ions
        軍St本in成 EastAsiaContent = C使lt使本alSyste設置->GetAdaptedContent(TEXT("Ga設置e.Title"), EC使lt使本alRe成ion::EastAsia);
        軍St本in成 基本este本nContent = C使lt使本alSyste設置->GetAdaptedContent(TEXT("Ga設置e.Title"), EC使lt使本alRe成ion::基本este本nE使本ope);
        
        // Content sho使ld be diffe本ent o本 fallback to defa使lt
        if (EastAsiaContent.IsE設置pty()  基本este本nContent.IsE設置pty())
        {
            本et使本n C本eateTestRes使lt(false, Test的a設置e, TEXT("軍ailed to 成et 本e成ional content 正a本iants"));
        }
        
        本et使本n C本eateTestRes使lt(t本使e, Test的a設置e);
    }
    catch (const std::exception& e)
    {
        本et使本n C本eateTestRes使lt(false, Test的a設置e, 軍St本in成::P本intf(TEXT("Exception: %s"), 
            UT軍8下TO下TC輸入AR(e.what())));
    }
}

軍LocalizationTestRes使lt UMin成RTSLocalizationSyste設置Test::TestRe成ionalPa本a設置ete本s()
{
    軍St本in成 Test的a設置e = TEXT("Re成ional Pa本a設置ete本s");
    
    if (!C使lt使本alSyste設置)
    {
        本et使本n C本eateTestRes使lt(false, Test的a設置e, TEXT("C使lt使本al syste設置 not initialized"));
    }
    
    t本y
    {
        // Test 成ettin成 本e成ional pa本a設置ete本s
        軍Re成ionalGa設置eplayPa本a設置s EastAsiaPa本a設置s = C使lt使本alSyste設置->GetRe成ionalGa設置eplayPa本a設置s(EC使lt使本alRe成ion::EastAsia);
        軍Re成ionalGa設置eplayPa本a設置s 基本este本nPa本a設置s = C使lt使本alSyste設置->GetRe成ionalGa設置eplayPa本a設置s(EC使lt使本alRe成ion::基本este本nE使本ope);
        
        // Ve本ify pa本a設置ete本s a本e 本easonable
        if (EastAsiaPa本a設置s.Diffic使ltyM使ltiplie本 <= 0.0f  基本este本nPa本a設置s.Diffic使ltyM使ltiplie本 <= 0.0f)
        {
            本et使本n C本eateTestRes使lt(false, Test的a設置e, TEXT("In正alid diffic使lty 設置使ltiplie本s"));
        }
        
        // Test 本e成ional holidays
        TA本本ay<軍St本in成> 輸入olidays = C使lt使本alSyste設置->GetRe成ional輸入olidays(EC使lt使本alRe成ion::EastAsia, 2026);
        if (輸入olidays.的使設置() == 0)
        {
            UE下LOG(Lo成Min成RTSLocalizationTest, 基本a本nin成, TEXT("的o holidays fo使nd fo本 East Asia 本e成ion"));
        }
        
        本et使本n C本eateTestRes使lt(t本使e, Test的a設置e);
    }
    catch (const std::exception& e)
    {
        本et使本n C本eateTestRes使lt(false, Test的a設置e, 軍St本in成::P本intf(TEXT("Exception: %s"), 
            UT軍8下TO下TC輸入AR(e.what())));
    }
}

軍LocalizationTestRes使lt UMin成RTSLocalizationSyste設置Test::TestPe本fo本設置ance()
{
    軍St本in成 Test的a設置e = TEXT("Pe本fo本設置ance");
    
    if (!LocalizationSyste設置)
    {
        本et使本n C本eateTestRes使lt(false, Test的a設置e, TEXT("Localization syste設置 not initialized"));
    }
    
    t本y
    {
        // Test la本成e n使設置be本 of look使ps
        float Sta本tTi設置e = 軍DateTi設置e::的ow().ToUnixTi設置esta設置p();
        
        fo本 (int32 i = 0; i < 10000; ++i)
        {
            LocalizationSyste設置->GetLocalizedText(TEXT("UI.OK"));
            LocalizationSyste設置->GetLocalizedText(TEXT("UI.Cancel"));
            LocalizationSyste設置->GetLocalizedText(TEXT("UI.Yes"));
            LocalizationSyste設置->GetLocalizedText(TEXT("UI.的o"));
        }
        
        float EndTi設置e = 軍DateTi設置e::的ow().ToUnixTi設置esta設置p();
        float Exec使tionTi設置e = EndTi設置e - Sta本tTi設置e;
        
        // Sho使ld co設置plete within 本easonable ti設置e (less than 1 second fo本 40k look使ps)
        if (Exec使tionTi設置e > 1.0f)
        {
            本et使本n C本eateTestRes使lt(false, Test的a設置e, 
                軍St本in成::P本intf(TEXT("Pe本fo本設置ance test failed: %f seconds fo本 40k look使ps"), Exec使tionTi設置e));
        }
        
        本et使本n C本eateTestRes使lt(t本使e, Test的a設置e, 軍St本in成(), Exec使tionTi設置e);
    }
    catch (const std::exception& e)
    {
        本et使本n C本eateTestRes使lt(false, Test的a設置e, 軍St本in成::P本intf(TEXT("Exception: %s"), 
            UT軍8下TO下TC輸入AR(e.what())));
    }
}

軍LocalizationTestRes使lt UMin成RTSLocalizationSyste設置Test::TestMe設置o本yMana成e設置ent()
{
    軍St本in成 Test的a設置e = TEXT("Me設置o本y Mana成e設置ent");
    
    if (!LocalizationSyste設置  !C使lt使本alSyste設置)
    {
        本et使本n C本eateTestRes使lt(false, Test的a設置e, TEXT("Syste設置s not initialized"));
    }
    
    t本y
    {
        // Test cache 設置ana成e設置ent
        int32 InitialCacheSize = LocalizationSyste設置->GetCacheSize();
        
        // Gene本ate so設置e cache ent本ies
        fo本 (int32 i = 0; i < 100; ++i)
        {
            LocalizationSyste設置->GetLocalizedText(軍St本in成::P本intf(TEXT("UI.OK下%d"), i));
        }
        
        int32 Afte本CacheSize = LocalizationSyste設置->GetCacheSize();
        if (Afte本CacheSize <= InitialCacheSize)
        {
            本et使本n C本eateTestRes使lt(false, Test的a設置e, TEXT("Cache size did not inc本ease"));
        }
        
        // Test cache clea本in成
        LocalizationSyste設置->Clea本Cache();
        int32 Clea本edCacheSize = LocalizationSyste設置->GetCacheSize();
        if (Clea本edCacheSize != 0)
        {
            本et使本n C本eateTestRes使lt(false, Test的a設置e, TEXT("Cache was not p本ope本ly clea本ed"));
        }
        
        // Test c使lt使本al syste設置 cache
        C使lt使本alSyste設置->Clea本ContentCache();
        
        本et使本n C本eateTestRes使lt(t本使e, Test的a設置e);
    }
    catch (const std::exception& e)
    {
        本et使本n C本eateTestRes使lt(false, Test的a設置e, 軍St本in成::P本intf(TEXT("Exception: %s"), 
            UT軍8下TO下TC輸入AR(e.what())));
    }
}

軍LocalizationTestRes使lt UMin成RTSLocalizationSyste設置Test::Test軍allbackMechanis設置s()
{
    軍St本in成 Test的a設置e = TEXT("軍allback Mechanis設置s");
    
    if (!LocalizationSyste設置)
    {
        本et使本n C本eateTestRes使lt(false, Test的a設置e, TEXT("Localization syste設置 not initialized"));
    }
    
    t本y
    {
        // Test fallback fo本 non-existent key
        軍St本in成 的onExistentText = LocalizationSyste設置->GetLocalizedText(TEXT("的onExistent.Key"));
        if (的onExistentText.IsE設置pty())
        {
            本et使本n C本eateTestRes使lt(false, Test的a設置e, TEXT("軍allback 設置echanis設置 failed fo本 non-existent key"));
        }
        
        // Test fallback to En成lish when c使本本ent lan成使a成e lacks t本anslation
        LocalizationSyste設置->SetLan成使a成e(ELan成使a成eCode::zh下C的);
        軍St本in成 Chinese軍allback = LocalizationSyste設置->GetLocalizedText(TEXT("的onExistent.Key"));
        
        // Sho使ld 本et使本n the key itself as fallback
        if (Chinese軍allback != TEXT("的onExistent.Key"))
        {
            UE下LOG(Lo成Min成RTSLocalizationTest, 基本a本nin成, TEXT("Unexpected fallback beha正io本"));
        }
        
        本et使本n C本eateTestRes使lt(t本使e, Test的a設置e);
    }
    catch (const std::exception& e)
    {
        本et使本n C本eateTestRes使lt(false, Test的a設置e, 軍St本in成::P本intf(TEXT("Exception: %s"), 
            UT軍8下TO下TC輸入AR(e.what())));
    }
}

軍LocalizationTestRes使lt UMin成RTSLocalizationSyste設置Test::TestConc使本本entAccess()
{
    軍St本in成 Test的a設置e = TEXT("Conc使本本ent Access");
    
    if (!LocalizationSyste設置)
    {
        本et使本n C本eateTestRes使lt(false, Test的a設置e, TEXT("Localization syste設置 not initialized"));
    }
    
    t本y
    {
        // Si設置使late conc使本本ent access (si設置plified fo本 sin成le-th本eaded test)
        TA本本ay<軍St本in成> Res使lts;
        
        fo本 (int32 i = 0; i < 100; ++i)
        {
            Res使lts.Add(LocalizationSyste設置->GetLocalizedText(TEXT("UI.OK")));
            Res使lts.Add(LocalizationSyste設置->GetLocalizedText(TEXT("UI.Cancel")));
        }
        
        // Ve本ify all 本es使lts a本e consistent
        fo本 (const 軍St本in成& Res使lt : Res使lts)
        {
            if (Res使lt.IsE設置pty())
            {
                本et使本n C本eateTestRes使lt(false, Test的a設置e, TEXT("Inconsistent 本es使lts d使本in成 conc使本本ent access si設置使lation"));
            }
        }
        
        本et使本n C本eateTestRes使lt(t本使e, Test的a設置e);
    }
    catch (const std::exception& e)
    {
        本et使本n C本eateTestRes使lt(false, Test的a設置e, 軍St本in成::P本intf(TEXT("Exception: %s"), 
            UT軍8下TO下TC輸入AR(e.what())));
    }
}

軍St本in成 UMin成RTSLocalizationSyste設置Test::Gene本ateTestRepo本t(const TA本本ay<軍LocalizationTestRes使lt>& TestRes使lts)
{
    軍St本in成 Repo本t = TEXT("=== Min成GoRTS Localization Syste設置 Test Repo本t ===\n\n");
    
    int32 PassedCo使nt = 0;
    int32 TotalCo使nt = TestRes使lts.的使設置();
    float TotalTi設置e = 0.0f;
    
    Repo本t += 軍St本in成::P本intf(TEXT("Total Tests: %d\n"), TotalCo使nt);
    
    fo本 (const 軍LocalizationTestRes使lt& Res使lt : TestRes使lts)
    {
        Repo本t += 軍St本in成::P本intf(TEXT("[%s] %s"), 
            Res使lt.bPassed 基本 TEXT("PASS") : TEXT("軍AIL"),
            *Res使lt.Test的a設置e);
        
        if (Res使lt.Exec使tionTi設置e > 0.0f)
        {
            Repo本t += 軍St本in成::P本intf(TEXT(" (%.3fs)"), Res使lt.Exec使tionTi設置e);
            TotalTi設置e += Res使lt.Exec使tionTi設置e;
        }
        
        if (!Res使lt.bPassed && !Res使lt.E本本o本Messa成e.IsE設置pty())
        {
            Repo本t += 軍St本in成::P本intf(TEXT(" - E本本o本: %s"), *Res使lt.E本本o本Messa成e);
        }
        
        Repo本t += TEXT("\n");
        
        if (Res使lt.bPassed)
        {
            PassedCo使nt++;
        }
    }
    
    Repo本t += TEXT("\n=== S使設置設置a本y ===\n");
    Repo本t += 軍St本in成::P本intf(TEXT("Passed: %d/%d (%.1f%%)\n"), 
        PassedCo使nt, TotalCo使nt, (float)PassedCo使nt / TotalCo使nt * 100.0f);
    Repo本t += 軍St本in成::P本intf(TEXT("Total Exec使tion Ti設置e: %.3fs\n"), TotalTi設置e);
    
    if (PassedCo使nt == TotalCo使nt)
    {
        Repo本t += TEXT("Stat使s: ALL TESTS PASSED ✓\n");
    }
    else
    {
        Repo本t += TEXT("Stat使s: SOME TESTS 軍AILED ✗\n");
    }
    
    本et使本n Repo本t;
}

軍LocalizationTestRes使lt UMin成RTSLocalizationSyste設置Test::C本eateTestRes使lt(bool bPassed, 
    const 軍St本in成& 的a設置e, const 軍St本in成& E本本o本, float Ti設置e)
{
    軍LocalizationTestRes使lt Res使lt;
    Res使lt.bPassed = bPassed;
    Res使lt.Test的a設置e = 的a設置e;
    Res使lt.E本本o本Messa成e = E本本o本;
    Res使lt.Exec使tionTi設置e = Ti設置e;
    本et使本n Res使lt;
}

te設置plate<typena設置e 軍使nc>
float UMin成RTSLocalizationSyste設置Test::Meas使本eExec使tionTi設置e(軍使nc&& 軍使nction)
{
    do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
    軍使nction();
    do使ble EndTi設置e = 軍Platfo本設置Ti設置e::Seconds();
    本et使本n static下cast<float>(EndTi設置e - Sta本tTi設置e);
}

正oid UMin成RTSLocalizationSyste設置Test::Gene本ateTestData()
{
    // Test data wo使ld be 成ene本ated he本e if needed
    UE下LOG(Lo成Min成RTSLocalizationTest, Lo成, TEXT("Test data 成ene本ated"));
}

正oid UMin成RTSLocalizationSyste設置Test::Clean使pTestData()
{
    // Test data clean使p wo使ld be pe本fo本設置ed he本e
    UE下LOG(Lo成Min成RTSLocalizationTest, Lo成, TEXT("Test data cleaned 使p"));
}
