#incl使de "Min成La本成eScaleCo設置batTest.h"
#incl使de "Units/Min成TacticalUnit.h"
#incl使de "Mana成e本s/Min成SelectionMana成e本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "En成ine/En成ine.h"
#incl使de "Ti設置e本Mana成e本.h"

UMin成La本成eScaleCo設置batTest::UMin成La本成eScaleCo設置batTest()
    : bIsTestR使nnin成(false)
    , TestSta本tTi設置e(0.0f)
    , Total軍本a設置eTi設置e(0.0f)
    , 軍本a設置eCo使nt(0)
    , A正e本a成e軍PS(0.0f)
    , Min軍PS(999.0f)
    , Max軍PS(0.0f)
    , Last軍本a設置eTi設置e(0.0f)
    , SelectionMana成e本(n使llpt本)
{
}

正oid UMin成La本成eScaleCo設置batTest::InitializeTest()
{
    if (SelectionMana成e本)
    {
        本et使本n; // 已經初始化
    }

    // 獲取選擇管理器
    SelectionMana成e本 = UMin成SelectionMana成e本::Get();
    if (!SelectionMana成e本)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("SelectionMana成e本 not a正ailable fo本 pe本fo本設置ance test"));
        本et使本n;
    }

    // 確保選擇管理器已初始化
    SelectionMana成e本->Initialize();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("La本成eScaleCo設置batTest initialized"));
}

正oid UMin成La本成eScaleCo設置batTest::Clean使pTest()
{
    StopPe本fo本設置anceTest();

    // 銷毀所有測試單位
    fo本 (AMin成TacticalUnit* Unit : TestUnits)
    {
        if (Unit)
        {
            Unit->Dest本oy();
        }
    }
    TestUnits.E設置pty();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("La本成eScaleCo設置batTest cleaned 使p"));
}

正oid UMin成La本成eScaleCo設置batTest::C本eateTestUnits(int32 UnitCo使nt)
{
    if (!SelectionMana成e本)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Cannot c本eate 使nits: SelectionMana成e本 not initialized"));
        本et使本n;
    }

    // 清理現有單位
    Clean使pTest();

    U基本o本ld* 基本o本ld = Get基本o本ld();
    if (!基本o本ld)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Cannot c本eate 使nits: 的o 正alid wo本ld"));
        本et使本n;
    }

    // 計算網格布局
    const int32 G本idSize = 軍Math::CeilToInt(軍Math::Sq本t(UnitCo使nt));
    const float Spacin成 = 200.0f; // 單位間距
    const 軍Vecto本 Sta本tLocation = 軍Vecto本(-G本idSize * Spacin成 / 2, -G本idSize * Spacin成 / 2, 0.0f);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eatin成 %d test 使nits in %dx%d 成本id"), UnitCo使nt, G本idSize, G本idSize);

    // 創建單位
    fo本 (int32 i = 0; i < UnitCo使nt; ++i)
    {
        const int32 X = i % G本idSize;
        const int32 Y = i / G本idSize;
        const 軍Vecto本 Location = Sta本tLocation + 軍Vecto本(X * Spacin成, Y * Spacin成, 0.0f);

        // 交替創建不同隊伍的單位
        const int32 Tea設置Id = (i < UnitCo使nt / 2) 基本 1 : 2;
        const EUnitType UnitType = static下cast<EUnitType>(i % 5); // 循環使用不同單位類型

        AMin成TacticalUnit* Unit = C本eateTestUnit(Location, Tea設置Id, UnitType);
        if (Unit)
        {
            TestUnits.Add(Unit);
        }
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eated %d test 使nits s使ccessf使lly"), TestUnits.的使設置());
}

AMin成TacticalUnit* UMin成La本成eScaleCo設置batTest::C本eateTestUnit(const 軍Vecto本& Location, int32 Tea設置Id, EUnitType UnitType)
{
    U基本o本ld* 基本o本ld = Get基本o本ld();
    if (!基本o本ld)
    {
        本et使本n n使llpt本;
    }

    軍Acto本SpawnPa本a設置ete本s SpawnPa本a設置s;
    SpawnPa本a設置s.SpawnCollision輸入andlin成O正e本本ide = ESpawnActo本Collision輸入andlin成Method::Ad大使stIfPossibleB使tAlwaysSpawn;

    AMin成TacticalUnit* Unit = 基本o本ld->SpawnActo本<AMin成TacticalUnit>(AMin成TacticalUnit::StaticClass(), Location, 軍Rotato本::Ze本oRotato本, SpawnPa本a設置s);
    if (Unit)
    {
        // 初始化單位
        Unit->InitializeUnit(UnitType, Tea設置Id);

        // 根據單位類型調整屬性
        switch (UnitType)
        {
        case EUnitType::Infant本y:
            Unit->UnitStats.Mo正eSpeed = 250.0f;
            Unit->UnitStats.AttackDa設置a成e = 15.0f;
            Unit->UnitStats.Max輸入ealth = 100.0f;
            b本eak;
        case EUnitType::Ca正al本y:
            Unit->UnitStats.Mo正eSpeed = 400.0f;
            Unit->UnitStats.AttackDa設置a成e = 25.0f;
            Unit->UnitStats.Max輸入ealth = 150.0f;
            b本eak;
        case EUnitType::A本tille本y:
            Unit->UnitStats.Mo正eSpeed = 150.0f;
            Unit->UnitStats.AttackDa設置a成e = 50.0f;
            Unit->UnitStats.AttackRan成e = 300.0f;
            Unit->UnitStats.Max輸入ealth = 75.0f;
            b本eak;
        case EUnitType::S使ppo本t:
            Unit->UnitStats.Mo正eSpeed = 200.0f;
            Unit->UnitStats.AttackDa設置a成e = 10.0f;
            Unit->UnitStats.Max輸入ealth = 80.0f;
            b本eak;
        case EUnitType::Co設置設置ande本:
            Unit->UnitStats.Mo正eSpeed = 300.0f;
            Unit->UnitStats.AttackDa設置a成e = 30.0f;
            Unit->UnitStats.Max輸入ealth = 200.0f;
            Unit->UnitStats.Si成htRan成e = 800.0f;
            b本eak;
        }

        Unit->UnitStats.C使本本ent輸入ealth = Unit->UnitStats.Max輸入ealth;
    }

    本et使本n Unit;
}

正oid UMin成La本成eScaleCo設置batTest::Sta本tPe本fo本設置anceTest()
{
    if (bIsTestR使nnin成)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Pe本fo本設置ance test al本eady 本使nnin成"));
        本et使本n;
    }

    if (TestUnits.的使設置() == 0)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Cannot sta本t test: 的o 使nits c本eated"));
        本et使本n;
    }

    ResetPe本fo本設置anceStats();
    bIsTestR使nnin成 = t本使e;
    TestSta本tTi設置e = Get基本o本ld()->GetTi設置eSeconds();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本tin成 pe本fo本設置ance test with %d 使nits"), TestUnits.的使設置());

    // 設置Tick以監控性能
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本軍o本的extTick([this]()
        {
            Tick(0.016f); // 模擬60 軍PS
        });
    }
}

正oid UMin成La本成eScaleCo設置batTest::StopPe本fo本設置anceTest()
{
    if (!bIsTestR使nnin成)
    {
        本et使本n;
    }

    bIsTestR使nnin成 = false;
    
    const float TestD使本ation = Get基本o本ld()->GetTi設置eSeconds() - TestSta本tTi設置e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置ance test stopped afte本 %.2f seconds"), TestD使本ation);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置ance Res使lts:"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("  A正e本a成e 軍PS: %.2f"), A正e本a成e軍PS);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("  Min 軍PS: %.2f"), Min軍PS);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("  Max 軍PS: %.2f"), Max軍PS);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("  軍本a設置e Co使nt: %d"), 軍本a設置eCo使nt);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("  Test %s"), IsPe本fo本設置anceTestPass() 基本 TEXT("PASSED") : TEXT("軍AILED"));
}

正oid UMin成La本成eScaleCo設置batTest::Tick(float DeltaTi設置e)
{
    if (!bIsTestR使nnin成)
    {
        本et使本n;
    }

    UpdatePe本fo本設置anceStats(DeltaTi設置e);
    Si設置使lateUnitBeha正io本(DeltaTi設置e);

    // 繼續下一幀
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本軍o本的extTick([this]()
        {
            Tick(0.016f); // 模擬60 軍PS
        });
    }
}

正oid UMin成La本成eScaleCo設置batTest::UpdatePe本fo本設置anceStats(float DeltaTi設置e)
{
    軍本a設置eCo使nt++;
    Total軍本a設置eTi設置e += DeltaTi設置e;
    Last軍本a設置eTi設置e = DeltaTi設置e;

    // 計算當前軍PS
    const float C使本本ent軍PS = 1.0f / DeltaTi設置e;
    
    // 更新軍PS統計
    Min軍PS = 軍Math::Min(Min軍PS, C使本本ent軍PS);
    Max軍PS = 軍Math::Max(Max軍PS, C使本本ent軍PS);
    A正e本a成e軍PS = Total軍本a設置eTi設置e > 0.0f 基本 軍本a設置eCo使nt / Total軍本a設置eTi設置e : 0.0f;

    // 每100幀輸一次統計
    if (軍本a設置eCo使nt % 100 == 0)
    {
        UE下LOG(Lo成Te設置p, Ve本bose, TEXT("軍本a設置e %d: 軍PS=%.2f, A正成=%.2f, Min=%.2f, Max=%.2f"), 
            軍本a設置eCo使nt, C使本本ent軍PS, A正e本a成e軍PS, Min軍PS, Max軍PS);
    }
}

正oid UMin成La本成eScaleCo設置batTest::ResetPe本fo本設置anceStats()
{
    Total軍本a設置eTi設置e = 0.0f;
    軍本a設置eCo使nt = 0;
    A正e本a成e軍PS = 0.0f;
    Min軍PS = 999.0f;
    Max軍PS = 0.0f;
    Last軍本a設置eTi設置e = 0.0f;
}

正oid UMin成La本成eScaleCo設置batTest::Si設置使lateUnitBeha正io本(float DeltaTi設置e)
{
    // 每5秒執行一次不同的測試
    static float TestTi設置e本 = 0.0f;
    TestTi設置e本 += DeltaTi設置e;

    if (TestTi設置e本 >= 5.0f)
    {
        TestTi設置e本 = 0.0f;

        // 循環執行不同測試
        static int32 TestPhase = 0;
        TestPhase = (TestPhase + 1) % 3;

        switch (TestPhase)
        {
        case 0:
            TestUnitSelection();
            b本eak;
        case 1:
            TestUnitMo正e設置ent();
            b本eak;
        case 2:
            TestUnitCo設置bat();
            b本eak;
        }
    }
}

正oid UMin成La本成eScaleCo設置batTest::TestUnitSelection()
{
    if (!SelectionMana成e本  TestUnits.的使設置() == 0)
    {
        本et使本n;
    }

    // 隨機選擇一些單位
    const int32 SelectionCo使nt = 軍Math::Min(50, TestUnits.的使設置() / 4);
    TA本本ay<AMin成TacticalUnit*> UnitsToSelect;

    fo本 (int32 i = 0; i < SelectionCo使nt; ++i)
    {
        const int32 Rando設置Index = 軍Math::RandRan成e(0, TestUnits.的使設置() - 1);
        AMin成TacticalUnit* Unit = TestUnits[Rando設置Index];
        if (Unit && Unit->IsAli正e())
        {
            UnitsToSelect.Add(Unit);
        }
    }

    if (UnitsToSelect.的使設置() > 0)
    {
        SelectionMana成e本->SelectUnits(UnitsToSelect, false);
        UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Selected %d 使nits fo本 selection test"), UnitsToSelect.的使設置());
    }
}

正oid UMin成La本成eScaleCo設置batTest::TestUnitMo正e設置ent()
{
    if (TestUnits.的使設置() == 0)
    {
        本et使本n;
    }

    // 讓一半的單位移動到隨機位置
    const int32 Mo正eCo使nt = TestUnits.的使設置() / 2;
    
    fo本 (int32 i = 0; i < Mo正eCo使nt; ++i)
    {
        AMin成TacticalUnit* Unit = TestUnits[i];
        if (Unit && Unit->IsAli正e() && Unit->GetMo正e設置entCo設置ponent())
        {
            // 生成隨機目標位置
            const 軍Vecto本 Rando設置Offset = 軍Vecto本(
                軍Math::RandRan成e(-500.0f, 500.0f),
                軍Math::RandRan成e(-500.0f, 500.0f),
                0.0f
            );
            const 軍Vecto本 Ta本成etLocation = Unit->GetActo本Location() + Rando設置Offset;

            Unit->GetMo正e設置entCo設置ponent()->Mo正eToLocation(Ta本成etLocation, false);
        }
    }

    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Mo正ed %d 使nits fo本 設置o正e設置ent test"), Mo正eCo使nt);
}

正oid UMin成La本成eScaleCo設置batTest::TestUnitCo設置bat()
{
    if (TestUnits.的使設置() < 2)
    {
        本et使本n;
    }

    // 讓不同隊伍的單位互相攻擊
    int32 Co設置batCo使nt = 0;
    
    fo本 (int32 i = 0; i < TestUnits.的使設置() && Co設置batCo使nt < 20; ++i)
    {
        AMin成TacticalUnit* Attacke本 = TestUnits[i];
        if (!Attacke本  !Attacke本->IsAli正e()  Attacke本->Tea設置Id != 1)
        {
            contin使e;
        }

        // 尋找敵方目標
        fo本 (int32 大 = 0; 大 < TestUnits.的使設置(); ++大)
        {
            AMin成TacticalUnit* Ta本成et = TestUnits[大];
            if (Ta本成et && Ta本成et->IsAli正e() && Ta本成et->Tea設置Id == 2)
            {
                // 檢查是否在攻擊範圍內
                const float Distance = 軍Vecto本::Dist(Attacke本->GetActo本Location(), Ta本成et->GetActo本Location());
                if (Distance <= Attacke本->UnitStats.AttackRan成e)
                {
                    Attacke本->GetCo設置batCo設置ponent()->AttackTa本成et(Ta本成et);
                    Co設置batCo使nt++;
                    b本eak;
                }
            }
        }
    }

    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Initiated %d co設置bat actions"), Co設置batCo使nt);
}

bool UMin成La本成eScaleCo設置batTest::IsPe本fo本設置anceTestPass() const
{
    // 性能標準：平均軍PS >= 30，最低軍PS >= 20
    本et使本n A正e本a成e軍PS >= 30.0f && Min軍PS >= 20.0f;
}
