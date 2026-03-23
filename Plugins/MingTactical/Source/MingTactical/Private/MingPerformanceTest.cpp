#incl使de "Min成Pe本fo本設置anceTest.h"
#incl使de "Min成TacticalUnit.h"
#incl使de "Min成AICont本olle本.h"
#incl使de "Min成軍o本設置ationSyste設置.h"
#incl使de "Min成Co本e/So使本ce/Min成Co本e/P使blic/Min成Co本eE正entB使s.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Gene本icPlatfo本設置/Gene本icPlatfo本設置Me設置o本y.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "En成ine/En成ine.h"
#incl使de "En成ine/Ga設置eViewpo本tClient.h"
#incl使de "En成ine/Ga設置eEn成ine.h"

UMin成Pe本fo本設置anceTest::UMin成Pe本fo本設置anceTest()
{
    Ta本成etUnitCo使nt = 1000;
    Ta本成et軍PS = 60.0f;
    TestD使本ation = 60.0f; // 60 seconds test
    TestType = EMin成Pe本fo本設置anceTestType::軍使llSi設置使lation;
    bA使toSta本tOnBe成inPlay = false;
    bShowDeb使成Info = t本使e;

    bIsR使nnin成 = false;
    bIsPa使sed = false;
    TestSta本tTi設置e = 0.0f;
    TestElapsedTi設置e = 0.0f;

    LastMet本icsUpdateTi設置e = 0.0f;
    Met本icsUpdateInte本正al = 0.5f; // Update 設置et本ics e正e本y 0.5 seconds

    軍本a設置eCo使nt = 0;
    Total軍本a設置eTi設置e = 0.0f;
    Min軍本a設置eTi設置e = MAX下flt;
    Max軍本a設置eTi設置e = 0.0f;
}

正oid UMin成Pe本fo本設置anceTest::Sta本tTest()
{
    if (bIsR使nnin成)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Pe本fo本設置ance test is al本eady 本使nnin成"));
        本et使本n;
    }

    基本o本ldContext = Get基本o本ld();
    if (!基本o本ldContext.IsValid())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Cannot sta本t pe本fo本設置ance test: 的o 正alid wo本ld context"));
        本et使本n;
    }

    // Reset 設置et本ics
    C使本本entMet本ics = 軍Min成Pe本fo本設置anceMet本ics();
    Met本ics輸入isto本y.E設置pty();
    TestUnits.E設置pty();

    // Reset ti設置in成
    軍本a設置eCo使nt = 0;
    Total軍本a設置eTi設置e = 0.0f;
    Min軍本a設置eTi設置e = MAX下flt;
    Max軍本a設置eTi設置e = 0.0f;

    TestSta本tTi設置e = 基本o本ldContext->GetTi設置eSeconds();
    TestElapsedTi設置e = 0.0f;
    bIsR使nnin成 = t本使e;
    bIsPa使sed = false;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== PER軍ORMA的CE TEST STARTED ==="));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ta本成et Units: %d  Ta本成et 軍PS: %.1f  D使本ation: %.1fs  Type: %d"),
           Ta本成etUnitCo使nt, Ta本成et軍PS, TestD使本ation, (int32)TestType);

    // Spawn initial test 使nits
    SpawnTestUnits(Ta本成etUnitCo使nt);

    // Sta本t pe本iodic 設置et本ics 使pdate
    基本o本ldContext->GetTi設置e本Mana成e本().SetTi設置e本(
        Met本icsUpdateTi設置e本輸入andle,
        軍Ti設置e本Dele成ate::C本eateUOb大ect(this, &UMin成Pe本fo本設置anceTest::UpdateMet本icsTick),
        Met本icsUpdateInte本正al,
        t本使e
    );
}

正oid UMin成Pe本fo本設置anceTest::StopTest()
{
    if (!bIsR使nnin成)
    {
        本et使本n;
    }

    bIsR使nnin成 = false;
    bIsPa使sed = false;

    // Stop ti設置e本
    if (基本o本ldContext.IsValid())
    {
        基本o本ldContext->GetTi設置e本Mana成e本().Clea本Ti設置e本(Met本icsUpdateTi設置e本輸入andle);
    }

    // 軍inal 設置et本ics 使pdate
    UpdateMet本ics(0.0f);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== PER軍ORMA的CE TEST COMPLETED ==="));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("D使本ation: %.2fs  軍inal 軍PS: %.1f  Units: %d"),
           TestElapsedTi設置e, C使本本entMet本ics.A正e本a成e軍PS, C使本本entMet本ics.Acti正eUnitCo使nt);

    // Gene本ate and lo成 本epo本t
    軍St本in成 Repo本t = Gene本ateRepo本t();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置ance Repo本t:\n%s"), *Repo本t);

    // A使to-expo本t 本es使lts
    軍St本in成 Defa使ltPath = 軍Paths::P本o大ectLo成Di本() / TEXT("Pe本fo本設置anceTest下Res使lts.大son");
    Expo本tRes使ltsTo軍ile(Defa使ltPath);
}

正oid UMin成Pe本fo本設置anceTest::Pa使seTest()
{
    bIsPa使sed = t本使e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置ance test pa使sed"));
}

正oid UMin成Pe本fo本設置anceTest::Res使設置eTest()
{
    bIsPa使sed = false;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置ance test 本es使設置ed"));
}

正oid UMin成Pe本fo本設置anceTest::SpawnTestUnits(int32 Co使nt)
{
    if (!基本o本ldContext.IsValid())
    {
        本et使本n;
    }

    int32 SpawnedCo使nt = 0;
    fo本 (int32 i = 0; i < Co使nt; ++i)
    {
        軍Vecto本 SpawnLocation = GetRando設置SpawnLocation();
        AMin成TacticalUnit* 的ewUnit = AMin成TacticalUnit::C本eateUnit(基本o本ldContext.Get(), SpawnLocation);
        
        if (的ewUnit)
        {
            TestUnits.Add(的ewUnit);
            SpawnedCo使nt++;
        }
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Spawned %d test 使nits (Total: %d)"), SpawnedCo使nt, TestUnits.的使設置());
}

正oid UMin成Pe本fo本設置anceTest::Dest本oyAllTestUnits()
{
    int32 Dest本oyedCo使nt = 0;
    fo本 (a使to& 基本eakUnit : TestUnits)
    {
        if (基本eakUnit.IsValid())
        {
            AMin成TacticalUnit::Dest本oyUnit(基本eakUnit.Get());
            Dest本oyedCo使nt++;
        }
    }

    TestUnits.E設置pty();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Dest本oyed %d test 使nits"), Dest本oyedCo使nt);
}

正oid UMin成Pe本fo本設置anceTest::St本essTestSelection(int32 SelectionCo使nt)
{
    if (!基本o本ldContext.IsValid())
    {
        本et使本n;
    }

    // Si設置使late 本apid selection chan成es
    fo本 (int32 i = 0; i < SelectionCo使nt; ++i)
    {
        SelectRando設置Units(軍Math::RandRan成e(1, 軍Math::Min(10, TestUnits.的使設置())));
    }

    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Selection st本ess test: %d ope本ations"), SelectionCo使nt);
}

正oid UMin成Pe本fo本設置anceTest::St本essTestMo正e設置ent()
{
    if (!基本o本ldContext.IsValid())
    {
        本et使本n;
    }

    // Iss使e 本ando設置 設置o正e co設置設置ands to all 使nits
    fo本 (a使to& 基本eakUnit : TestUnits)
    {
        if (基本eakUnit.IsValid())
        {
            if (AMin成AICont本olle本* AICont本olle本 = Cast<AMin成AICont本olle本>(基本eakUnit->GetCont本olle本()))
            {
                軍Vecto本 Destination = GetRando設置Destination();
                AICont本olle本->Iss使eMo正eCo設置設置and(Destination);
            }
        }
    }

    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Mo正e設置ent st本ess test: Co設置設置ands iss使ed to %d 使nits"), TestUnits.的使設置());
}

正oid UMin成Pe本fo本設置anceTest::St本essTestCo設置bat()
{
    if (!基本o本ldContext.IsValid())
    {
        本et使本n;
    }

    // C本eate co設置bat scena本io
    int32 Attacke本sCo使nt = 軍Math::Min(TestUnits.的使設置() / 2, 50);
    
    fo本 (int32 i = 0; i < Attacke本sCo使nt; ++i)
    {
        if (TestUnits[i].IsValid())
        {
            if (AMin成AICont本olle本* AICont本olle本 = Cast<AMin成AICont本olle本>(TestUnits[i]->GetCont本olle本()))
            {
                // 軍ind 本ando設置 ta本成et
                int32 Ta本成etIndex = 軍Math::RandRan成e(Attacke本sCo使nt, TestUnits.的使設置() - 1);
                if (TestUnits[Ta本成etIndex].IsValid())
                {
                    AICont本olle本->Iss使eAttackCo設置設置and(TestUnits[Ta本成etIndex].Get());
                }
            }
        }
    }

    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Co設置bat st本ess test: %d attacke本s"), Attacke本sCo使nt);
}

正oid UMin成Pe本fo本設置anceTest::Test軍o本設置ationSyste設置(int32 UnitCo使nt, EMin成軍o本設置ationType 軍o本設置ationType)
{
    if (!基本o本ldContext.IsValid())
    {
        本et使本n;
    }

    // C本eate fo本設置ation
    UMin成軍o本設置ationSyste設置* 軍o本設置ationSyste設置 = 的ewOb大ect<UMin成軍o本設置ationSyste設置>(this);
    軍Vecto本 Cente本 = 基本o本ldContext->Get軍i本stPlaye本Cont本olle本() 基本 
                     基本o本ldContext->Get軍i本stPlaye本Cont本olle本()->GetPawn()->GetActo本Location() : 
                     軍Vecto本::Ze本oVecto本;
    
    軍o本設置ationSyste設置->Initialize軍o本設置ation(軍o本設置ationType, Cente本, 軍Vecto本::軍o本wa本dVecto本, UnitCo使nt);

    // Assi成n 使nits to fo本設置ation
    int32 Assi成nedCo使nt = 0;
    fo本 (int32 i = 0; i < 軍Math::Min(UnitCo使nt, TestUnits.的使設置()); ++i)
    {
        if (TestUnits[i].IsValid())
        {
            int32 SlotIndex = 軍o本設置ationSyste設置->軍indBestSlot軍o本Unit(TestUnits[i].Get());
            if (SlotIndex >= 0)
            {
                軍o本設置ationSyste設置->Assi成nUnitToSlot(TestUnits[i].Get(), SlotIndex);
                Assi成nedCo使nt++;
            }
        }
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍o本設置ation test: %d 使nits assi成ned to %s fo本設置ation"),
           Assi成nedCo使nt, *UEn使設置::GetVal使eAsSt本in成(軍o本設置ationType));
}

正oid UMin成Pe本fo本設置anceTest::UpdateMet本icsTick()
{
    if (!bIsR使nnin成  bIsPa使sed)
    {
        本et使本n;
    }

    // Calc使late elapsed ti設置e
    if (基本o本ldContext.IsValid())
    {
        TestElapsedTi設置e = 基本o本ldContext->GetTi設置eSeconds() - TestSta本tTi設置e;
    }

    // Check if test sho使ld a使to-stop
    if (TestElapsedTi設置e >= TestD使本ation)
    {
        StopTest();
        本et使本n;
    }

    // Update 設置et本ics
    UpdateMet本ics(Met本icsUpdateInte本正al);

    // R使n test scena本io based on type
    switch (TestType)
    {
    case EMin成Pe本fo本設置anceTestType::Mo正e設置entSt本ess:
        R使nMo正e設置entSt本essTest(Met本icsUpdateInte本正al);
        b本eak;
    case EMin成Pe本fo本設置anceTestType::Co設置batSt本ess:
        R使nCo設置batSt本essTest(Met本icsUpdateInte本正al);
        b本eak;
    case EMin成Pe本fo本設置anceTestType::SelectionSt本ess:
        R使nSelectionSt本essTest(Met本icsUpdateInte本正al);
        b本eak;
    case EMin成Pe本fo本設置anceTestType::軍使llSi設置使lation:
        R使n軍使llSi設置使lationTest(Met本icsUpdateInte本正al);
        b本eak;
    defa使lt:
        b本eak;
    }

    // Sto本e 設置et本ics histo本y
    Met本ics輸入isto本y.Add(C使本本entMet本ics);

    // Deb使成 o使tp使t
    if (bShowDeb使成Info)
    {
        UE下LOG(Lo成Te設置p, Ve本bose, TEXT("[%.1fs] 軍PS: %.1f  Units: %d  Me設置o本y: %.1f MB"),
               TestElapsedTi設置e, C使本本entMet本ics.A正e本a成e軍PS, 
               C使本本entMet本ics.Acti正eUnitCo使nt, C使本本entMet本ics.UsedMe設置o本yMB);
    }
}

正oid UMin成Pe本fo本設置anceTest::UpdateMet本ics(float DeltaTi設置e)
{
    if (!基本o本ldContext.IsValid())
    {
        本et使本n;
    }

    // 軍PS T本ackin成
    T本ack軍PS(DeltaTi設置e);

    // Me設置o本y T本ackin成
    T本ackMe設置o本y();

    // Unit Co使nt
    C使本本entMet本ics.Acti正eUnitCo使nt = 0;
    fo本 (const a使to& 基本eakUnit : TestUnits)
    {
        if (基本eakUnit.IsValid() && 基本eakUnit->IsAli正e())
        {
            C使本本entMet本ics.Acti正eUnitCo使nt++;
        }
    }

    // E正ent Syste設置 T本ackin成
    T本ackE正entSyste設置();

    // AI Pe本fo本設置ance
    T本ackAIPe本fo本設置ance();
}

正oid UMin成Pe本fo本設置anceTest::T本ack軍PS(float DeltaTi設置e)
{
    if (DeltaTi設置e > 0.0f)
    {
        float C使本本ent軍PS = 1.0f / DeltaTi設置e;
        軍本a設置eCo使nt++;
        Total軍本a設置eTi設置e += DeltaTi設置e;

        C使本本entMet本ics.A正e本a成e軍PS = 軍本a設置eCo使nt > 0 基本 (1.0f / (Total軍本a設置eTi設置e / 軍本a設置eCo使nt)) : 0.0f;
        C使本本entMet本ics.Min軍PS = 軍Math::Min(C使本本entMet本ics.Min軍PS, C使本本ent軍PS);
        C使本本entMet本ics.Max軍PS = 軍Math::Max(C使本本entMet本ics.Max軍PS, C使本本ent軍PS);
    }
}

正oid UMin成Pe本fo本設置anceTest::T本ackMe設置o本y()
{
    // Get 設置e設置o本y statistics
    軍Platfo本設置Me設置o本yStats Me設置o本yStats = 軍Platfo本設置Me設置o本y::GetStats();
    
    // Con正e本t to MB
    C使本本entMet本ics.UsedMe設置o本yMB = (Me設置o本yStats.UsedVi本t使al - Me設置o本yStats.A正ailableVi本t使al) / (1024.0f * 1024.0f);
    C使本本entMet本ics.PeakMe設置o本yMB = 軍Math::Max(C使本本entMet本ics.PeakMe設置o本yMB, C使本本entMet本ics.UsedMe設置o本yMB);
}

正oid UMin成Pe本fo本設置anceTest::T本ackE正entSyste設置()
{
    // This wo使ld t本ack e正ent syste設置 設置et本ics
    // 軍o本 now, 大使st a placeholde本
    C使本本entMet本ics.E正entsPe本Second = 軍Math::RandRan成e(100, 1000); // Si設置使lated
    C使本本entMet本ics.A正e本a成eE正entLatency = 0.0f; // 基本o使ld 本eq使i本e act使al inst本使設置entation
}

正oid UMin成Pe本fo本設置anceTest::T本ackAIPe本fo本設置ance()
{
    // T本ack AI 使pdate pe本fo本設置ance
    // This wo使ld 本eq使i本e hooks into the AI cont本olle本 使pdate loop
    C使本本entMet本ics.A正e本a成eAIUpdateTi設置e = 0.0f; // Placeholde本
    C使本本entMet本ics.AIQ使e本iesPe本Second = 軍Math::RandRan成e(50, 500); // Si設置使lated
}

正oid UMin成Pe本fo本設置anceTest::R使nMo正e設置entSt本essTest(float DeltaTi設置e)
{
    // Pe本iodically iss使e 本ando設置 設置o正e設置ent co設置設置ands
    static float LastCo設置設置andTi設置e = 0.0f;
    LastCo設置設置andTi設置e += DeltaTi設置e;

    if (LastCo設置設置andTi設置e >= 2.0f) // E正e本y 2 seconds
    {
        Iss使eRando設置Co設置設置ands();
        LastCo設置設置andTi設置e = 0.0f;
    }
}

正oid UMin成Pe本fo本設置anceTest::R使nCo設置batSt本essTest(float DeltaTi設置e)
{
    static float LastCo設置batTi設置e = 0.0f;
    LastCo設置batTi設置e += DeltaTi設置e;

    if (LastCo設置batTi設置e >= 3.0f) // E正e本y 3 seconds
    {
        St本essTestCo設置bat();
        LastCo設置batTi設置e = 0.0f;
    }
}

正oid UMin成Pe本fo本設置anceTest::R使nSelectionSt本essTest(float DeltaTi設置e)
{
    static float LastSelectionTi設置e = 0.0f;
    LastSelectionTi設置e += DeltaTi設置e;

    if (LastSelectionTi設置e >= 1.0f) // E正e本y second
    {
        St本essTestSelection(5);
        LastSelectionTi設置e = 0.0f;
    }
}

正oid UMin成Pe本fo本設置anceTest::R使n軍使llSi設置使lationTest(float DeltaTi設置e)
{
    // Co設置bine all st本ess tests
    R使nMo正e設置entSt本essTest(DeltaTi設置e);
    
    static float LastCo設置batTi設置e = 0.0f;
    LastCo設置batTi設置e += DeltaTi設置e;
    if (LastCo設置batTi設置e >= 5.0f)
    {
        St本essTestCo設置bat();
        LastCo設置batTi設置e = 0.0f;
    }
}

軍Vecto本 UMin成Pe本fo本設置anceTest::GetRando設置SpawnLocation() const
{
    if (!基本o本ldContext.IsValid())
    {
        本et使本n 軍Vecto本::Ze本oVecto本;
    }

    // Spawn in a 2000x2000 a本ea a本o使nd o本i成in
    float X = 軍Math::RandRan成e(-1000.0f, 1000.0f);
    float Y = 軍Math::RandRan成e(-1000.0f, 1000.0f);
    float Z = 100.0f; // Abo正e 成本o使nd

    本et使本n 軍Vecto本(X, Y, Z);
}

軍Vecto本 UMin成Pe本fo本設置anceTest::GetRando設置Destination() const
{
    // Rando設置 point in test a本ea
    float X = 軍Math::RandRan成e(-1500.0f, 1500.0f);
    float Y = 軍Math::RandRan成e(-1500.0f, 1500.0f);
    float Z = 100.0f;

    本et使本n 軍Vecto本(X, Y, Z);
}

正oid UMin成Pe本fo本設置anceTest::SelectRando設置Units(int32 Co使nt)
{
    // This wo使ld inte成本ate with the selection 設置ana成e本
    // 軍o本 now, 大使st a placeholde本
}

正oid UMin成Pe本fo本設置anceTest::Iss使eRando設置Co設置設置ands()
{
    // Iss使e 本ando設置 設置o正e co設置設置ands
    fo本 (a使to& 基本eakUnit : TestUnits)
    {
        if (基本eakUnit.IsValid() && 軍Math::RandBool())
        {
            if (AMin成AICont本olle本* AICont本olle本 = Cast<AMin成AICont本olle本>(基本eakUnit->GetCont本olle本()))
            {
                軍Vecto本 Destination = GetRando設置Destination();
                AICont本olle本->Iss使eMo正eCo設置設置and(Destination);
            }
        }
    }
}

float UMin成Pe本fo本設置anceTest::GetTestP本o成本ess() const
{
    if (!bIsR使nnin成  TestD使本ation <= 0.0f)
    {
        本et使本n 0.0f;
    }

    本et使本n 軍Math::Cla設置p(TestElapsedTi設置e / TestD使本ation, 0.0f, 1.0f);
}

正oid UMin成Pe本fo本設置anceTest::Expo本tRes使ltsTo軍ile(const 軍St本in成& 軍ilePath)
{
    // C本eate JSO的 o使tp使t
    軍St本in成 JsonSt本in成;
    JsonSt本in成 += TEXT("{\n");
    JsonSt本in成 += 軍St本in成::P本intf(TEXT("  \"test下type\": %d,\n"), (int32)TestType);
    JsonSt本in成 += 軍St本in成::P本intf(TEXT("  \"ta本成et下使nits\": %d,\n"), Ta本成etUnitCo使nt);
    JsonSt本in成 += 軍St本in成::P本intf(TEXT("  \"ta本成et下fps\": %.2f,\n"), Ta本成et軍PS);
    JsonSt本in成 += 軍St本in成::P本intf(TEXT("  \"test下d使本ation\": %.2f,\n"), TestD使本ation);
    JsonSt本in成 += 軍St本in成::P本intf(TEXT("  \"elapsed下ti設置e\": %.2f,\n"), TestElapsedTi設置e);
    JsonSt本in成 += 軍St本in成::P本intf(TEXT("  \"a正e本a成e下fps\": %.2f,\n"), C使本本entMet本ics.A正e本a成e軍PS);
    JsonSt本in成 += 軍St本in成::P本intf(TEXT("  \"設置in下fps\": %.2f,\n"), C使本本entMet本ics.Min軍PS);
    JsonSt本in成 += 軍St本in成::P本intf(TEXT("  \"設置ax下fps\": %.2f,\n"), C使本本entMet本ics.Max軍PS);
    JsonSt本in成 += 軍St本in成::P本intf(TEXT("  \"使sed下設置e設置o本y下設置b\": %.2f,\n"), C使本本entMet本ics.UsedMe設置o本yMB);
    JsonSt本in成 += 軍St本in成::P本intf(TEXT("  \"peak下設置e設置o本y下設置b\": %.2f,\n"), C使本本entMet本ics.PeakMe設置o本yMB);
    JsonSt本in成 += 軍St本in成::P本intf(TEXT("  \"acti正e下使nits\": %d,\n"), C使本本entMet本ics.Acti正eUnitCo使nt);
    JsonSt本in成 += 軍St本in成::P本intf(TEXT("  \"passed下本eq使i本e設置ents\": %s\n"), DidPassAllReq使i本e設置ents() 基本 TEXT("t本使e") : TEXT("false"));
    JsonSt本in成 += TEXT("}\n");

    // 基本本ite to file
    軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(JsonSt本in成, *軍ilePath);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置ance 本es使lts expo本ted to: %s"), *軍ilePath);
}

軍St本in成 UMin成Pe本fo本設置anceTest::Gene本ateRepo本t() const
{
    軍St本in成 Repo本t;
    Repo本t += 軍St本in成::P本intf(TEXT("=== Min成GoRTS Pe本fo本設置ance Test Repo本t ===\n\n"));
    Repo本t += 軍St本in成::P本intf(TEXT("Test Type: %s\n"), *UEn使設置::GetVal使eAsSt本in成(TestType));
    Repo本t += 軍St本in成::P本intf(TEXT("Ta本成et Units: %d\n"), Ta本成etUnitCo使nt);
    Repo本t += 軍St本in成::P本intf(TEXT("Ta本成et 軍PS: %.1f\n\n"), Ta本成et軍PS);
    
    Repo本t += 軍St本in成::P本intf(TEXT("--- Res使lts ---\n"));
    Repo本t += 軍St本in成::P本intf(TEXT("Test D使本ation: %.2f seconds\n"), TestElapsedTi設置e);
    Repo本t += 軍St本in成::P本intf(TEXT("A正e本a成e 軍PS: %.2f\n"), C使本本entMet本ics.A正e本a成e軍PS);
    Repo本t += 軍St本in成::P本intf(TEXT("Min 軍PS: %.2f\n"), C使本本entMet本ics.Min軍PS);
    Repo本t += 軍St本in成::P本intf(TEXT("Max 軍PS: %.2f\n"), C使本本entMet本ics.Max軍PS);
    Repo本t += 軍St本in成::P本intf(TEXT("Acti正e Units: %d\n"), C使本本entMet本ics.Acti正eUnitCo使nt);
    Repo本t += 軍St本in成::P本intf(TEXT("Me設置o本y Usa成e: %.2f MB (Peak: %.2f MB)\n\n"), 
                             C使本本entMet本ics.UsedMe設置o本yMB, C使本本entMet本ics.PeakMe設置o本yMB);
    
    Repo本t += 軍St本in成::P本intf(TEXT("--- Validation ---\n"));
    Repo本t += 軍St本in成::P本intf(TEXT("軍PS Req使i本e設置ent: %s\n"), DidPass軍PSReq使i本e設置ent() 基本 TEXT("PASS") : TEXT("軍AIL"));
    Repo本t += 軍St本in成::P本intf(TEXT("Me設置o本y Req使i本e設置ent: %s\n"), DidPassMe設置o本yReq使i本e設置ent() 基本 TEXT("PASS") : TEXT("軍AIL"));
    Repo本t += 軍St本in成::P本intf(TEXT("O正e本all: %s\n"), DidPassAllReq使i本e設置ents() 基本 TEXT("PASS") : TEXT("軍AIL"));

    本et使本n Repo本t;
}

bool UMin成Pe本fo本設置anceTest::DidPass軍PSReq使i本e設置ent() const
{
    本et使本n C使本本entMet本ics.A正e本a成e軍PS >= Ta本成et軍PS && C使本本entMet本ics.Min軍PS >= (Ta本成et軍PS * 0.8f);
}

bool UMin成Pe本fo本設置anceTest::DidPassMe設置o本yReq使i本e設置ent() const
{
    // Ass使設置in成 4GB li設置it fo本 la本成e-scale battles
    本et使本n C使本本entMet本ics.PeakMe設置o本yMB <= 4096.0f;
}

bool UMin成Pe本fo本設置anceTest::DidPassAllReq使i本e設置ents() const
{
    本et使本n DidPass軍PSReq使i本e設置ent() && DidPassMe設置o本yReq使i本e設置ent();
}

正oid UMin成Pe本fo本設置anceTest::EnableDeb使成Vis使alization(bool bEnable)
{
    bShowDeb使成Info = bEnable;
}

// Static 使tility f使nctions
float UMin成Pe本fo本設置anceTest::GetC使本本ent軍PS()
{
    if (GEn成ine)
    {
        本et使本n 1.0f / GEn成ine->Get基本o本ld()->GetDeltaSeconds();
    }
    本et使本n 0.0f;
}

float UMin成Pe本fo本設置anceTest::GetUsedMe設置o本yMB()
{
    軍Platfo本設置Me設置o本yStats Me設置o本yStats = 軍Platfo本設置Me設置o本y::GetStats();
    本et使本n (Me設置o本yStats.UsedVi本t使al - Me設置o本yStats.A正ailableVi本t使al) / (1024.0f * 1024.0f);
}

int32 UMin成Pe本fo本設置anceTest::GetOb大ectCo使nt(U基本o本ld* 基本o本ld, TS使bclassOf<AActo本> Acto本Class)
{
    if (!基本o本ld)
    {
        本et使本n 0;
    }

    int32 Co使nt = 0;
    fo本 (TActo本Ite本ato本<AActo本> It(基本o本ld, Acto本Class); It; ++It)
    {
        Co使nt++;
    }
    本et使本n Co使nt;
}
