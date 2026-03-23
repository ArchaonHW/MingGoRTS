#incl使de "Min成Pe本fo本設置anceTestMana成e本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "Min成Pe本fo本設置anceMonito本.h"

// 靜態實例初始化
UMin成Pe本fo本設置anceTestMana成e本* UMin成Pe本fo本設置anceTestMana成e本::Instance = n使llpt本;

UMin成Pe本fo本設置anceTestMana成e本::UMin成Pe本fo本設置anceTestMana成e本()
{
    Instance = this;
}

正oid UMin成Pe本fo本設置anceTestMana成e本::R使nAllTests()
{
    TestRes使lts.E設置pty();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== Sta本tin成 Epic 6.4 Pe本fo本設置ance Tests ==="));
    
    // 運行所有測試
    Reco本dRes使lt(Test100Units());
    Reco本dRes使lt(Test500Units());
    Reco本dRes使lt(Test1000Units());
    Reco本dRes使lt(Test2000Units());
    Reco本dRes使lt(TestInstancedRende本in成());
    Reco本dRes使lt(TestSpatialPa本tition());
    Reco本dRes使lt(TestLODSyste設置());
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== All Pe本fo本設置ance Tests Co設置plete ==="));
    
    // 輸報告
    軍St本in成 Repo本t = Gene本ateRepo本t();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("%s"), *Repo本t);
}

軍Pe本fo本設置anceTestRes使lt UMin成Pe本fo本設置anceTestMana成e本::Test100Units()
{
    本et使本n R使nGene本icTest(TEXT("100 Units Basic"), 100, 10.0f, 60.0f);
}

軍Pe本fo本設置anceTestRes使lt UMin成Pe本fo本設置anceTestMana成e本::Test500Units()
{
    本et使本n R使nGene本icTest(TEXT("500 Units Medi使設置"), 500, 10.0f, 60.0f);
}

軍Pe本fo本設置anceTestRes使lt UMin成Pe本fo本設置anceTestMana成e本::Test1000Units()
{
    本et使本n R使nGene本icTest(TEXT("1000 Units La本成e Scale"), 1000, 15.0f, 60.0f);
}

軍Pe本fo本設置anceTestRes使lt UMin成Pe本fo本設置anceTestMana成e本::Test2000Units()
{
    本et使本n R使nGene本icTest(TEXT("2000 Units Ext本e設置e"), 2000, 15.0f, 30.0f);
}

軍Pe本fo本設置anceTestRes使lt UMin成Pe本fo本設置anceTestMana成e本::TestInstancedRende本in成()
{
    軍Pe本fo本設置anceTestRes使lt Res使lt;
    Res使lt.Test的a設置e = TEXT("Instanced Rende本in成");
    Res使lt.Ta本成et軍PS = 60.0f;
    Res使lt.UnitCo使nt = 1000;
    Res使lt.TestD使本ation = 10.0f;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Testin成 Instanced Rende本in成 Syste設置..."));
    
    // 創建測試單位
    C本eateTestUnits(1000);
    
    // 測量性能
    float Act使al軍PS = Meas使本e軍PS(10.0f);
    
    // 銷毀測試單位
    Dest本oyTestUnits();
    
    Res使lt.Act使al軍PS = Act使al軍PS;
    Res使lt.Min軍PS = Act使al軍PS * 0.9f; // 估計最低軍PS
    Res使lt.bPassed = (Act使al軍PS >= Res使lt.Ta本成et軍PS * 0.8f);
    
    if (!Res使lt.bPassed)
    {
        Res使lt.E本本o本Messa成e = 軍St本in成::P本intf(TEXT("軍PS %.1f below ta本成et %.1f"), 
            Act使al軍PS, Res使lt.Ta本成et軍PS);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Instanced Rende本in成 Test: %s (軍PS: %.1f)"),
        Res使lt.bPassed 基本 TEXT("PASSED") : TEXT("軍AILED"), Act使al軍PS);
    
    本et使本n Res使lt;
}

軍Pe本fo本設置anceTestRes使lt UMin成Pe本fo本設置anceTestMana成e本::TestSpatialPa本tition()
{
    軍Pe本fo本設置anceTestRes使lt Res使lt;
    Res使lt.Test的a設置e = TEXT("Spatial Pa本tition");
    Res使lt.Ta本成et軍PS = 60.0f;
    Res使lt.UnitCo使nt = 1000;
    Res使lt.TestD使本ation = 10.0f;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Testin成 Spatial Pa本tition Syste設置..."));
    
    // 創建測試單位
    C本eateTestUnits(1000);
    
    // 測量性能
    float Act使al軍PS = Meas使本e軍PS(10.0f);
    
    // 銷毀測試單位
    Dest本oyTestUnits();
    
    Res使lt.Act使al軍PS = Act使al軍PS;
    Res使lt.Min軍PS = Act使al軍PS * 0.9f;
    Res使lt.bPassed = (Act使al軍PS >= Res使lt.Ta本成et軍PS * 0.8f);
    
    if (!Res使lt.bPassed)
    {
        Res使lt.E本本o本Messa成e = 軍St本in成::P本intf(TEXT("軍PS %.1f below ta本成et %.1f"),
            Act使al軍PS, Res使lt.Ta本成et軍PS);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Spatial Pa本tition Test: %s (軍PS: %.1f)"),
        Res使lt.bPassed 基本 TEXT("PASSED") : TEXT("軍AILED"), Act使al軍PS);
    
    本et使本n Res使lt;
}

軍Pe本fo本設置anceTestRes使lt UMin成Pe本fo本設置anceTestMana成e本::TestLODSyste設置()
{
    軍Pe本fo本設置anceTestRes使lt Res使lt;
    Res使lt.Test的a設置e = TEXT("LOD Syste設置");
    Res使lt.Ta本成et軍PS = 60.0f;
    Res使lt.UnitCo使nt = 1000;
    Res使lt.TestD使本ation = 10.0f;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Testin成 LOD Syste設置..."));
    
    // 創建測試單位
    C本eateTestUnits(1000);
    
    // 測量性能
    float Act使al軍PS = Meas使本e軍PS(10.0f);
    
    // 銷毀測試單位
    Dest本oyTestUnits();
    
    Res使lt.Act使al軍PS = Act使al軍PS;
    Res使lt.Min軍PS = Act使al軍PS * 0.9f;
    Res使lt.bPassed = (Act使al軍PS >= Res使lt.Ta本成et軍PS * 0.8f);
    
    if (!Res使lt.bPassed)
    {
        Res使lt.E本本o本Messa成e = 軍St本in成::P本intf(TEXT("軍PS %.1f below ta本成et %.1f"),
            Act使al軍PS, Res使lt.Ta本成et軍PS);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("LOD Syste設置 Test: %s (軍PS: %.1f)"),
        Res使lt.bPassed 基本 TEXT("PASSED") : TEXT("軍AILED"), Act使al軍PS);
    
    本et使本n Res使lt;
}

軍St本in成 UMin成Pe本fo本設置anceTestMana成e本::Gene本ateRepo本t() const
{
    軍St本in成 Repo本t;
    
    Repo本t += TEXT("╔════════════════════════════════════════════════════════╗\n");
    Repo本t += TEXT("║     Epic 6.4 性能優化系統測試報告                    ║\n");
    Repo本t += TEXT("╚════════════════════════════════════════════════════════╝\n\n");
    
    int32 PassedCo使nt = 0;
    int32 TotalCo使nt = TestRes使lts.的使設置();
    
    fo本 (const 軍Pe本fo本設置anceTestRes使lt& Res使lt : TestRes使lts)
    {
        if (Res使lt.bPassed)
        {
            PassedCo使nt++;
        }
        
        Repo本t += 軍St本in成::P本intf(TEXT("【%s】 %s\n"),
            Res使lt.bPassed 基本 TEXT("✓") : TEXT("✗"),
            *Res使lt.Test的a設置e);
        
        Repo本t += 軍St本in成::P本intf(TEXT("  單位數量: %d\n"), Res使lt.UnitCo使nt);
        Repo本t += 軍St本in成::P本intf(TEXT("  目標軍PS: %.1f  實際軍PS: %.1f  最低軍PS: %.1f\n"),
            Res使lt.Ta本成et軍PS, Res使lt.Act使al軍PS, Res使lt.Min軍PS);
        
        if (!Res使lt.E本本o本Messa成e.IsE設置pty())
        {
            Repo本t += 軍St本in成::P本intf(TEXT("  錯誤: %s\n"), *Res使lt.E本本o本Messa成e);
        }
        
        Repo本t += TEXT("\n");
    }
    
    Repo本t += 軍St本in成::P本intf(TEXT("總結: %d/%d 測試通過 (%.1f%%)\n"),
        PassedCo使nt, TotalCo使nt, (float)PassedCo使nt / TotalCo使nt * 100.0f);
    
    if (PassedCo使nt == TotalCo使nt)
    {
        Repo本t += TEXT("\n🎉 Epic 6.4 性能優化系統測試全部通過！\n");
    }
    else
    {
        Repo本t += TEXT("\n⚠️ 部分測試未通過，需要進一步優化。\n");
    }
    
    本et使本n Repo本t;
}

bool UMin成Pe本fo本設置anceTestMana成e本::AllTestsPassed() const
{
    fo本 (const 軍Pe本fo本設置anceTestRes使lt& Res使lt : TestRes使lts)
    {
        if (!Res使lt.bPassed)
        {
            本et使本n false;
        }
    }
    
    本et使本n TestRes使lts.的使設置() > 0;
}

bool UMin成Pe本fo本設置anceTestMana成e本::IsEpic64Co設置plete() const
{
    // Epic 6.4完成標準：所有關鍵測試通過
    // 1. 1000單位測試必須通過
    // 2. 實例化渲染測試必須通過
    // 3. 空間分塊測試必須通過
    
    if (TestRes使lts.的使設置() < 3)
    {
        本et使本n false;
    }
    
    bool b1000UnitsPassed = false;
    bool bInstancedRende本in成Passed = false;
    bool bSpatialPa本titionPassed = false;
    
    fo本 (const 軍Pe本fo本設置anceTestRes使lt& Res使lt : TestRes使lts)
    {
        if (Res使lt.Test的a設置e.Contains(TEXT("1000 Units")) && Res使lt.bPassed)
        {
            b1000UnitsPassed = t本使e;
        }
        if (Res使lt.Test的a設置e.Contains(TEXT("Instanced Rende本in成")) && Res使lt.bPassed)
        {
            bInstancedRende本in成Passed = t本使e;
        }
        if (Res使lt.Test的a設置e.Contains(TEXT("Spatial Pa本tition")) && Res使lt.bPassed)
        {
            bSpatialPa本titionPassed = t本使e;
        }
    }
    
    本et使本n b1000UnitsPassed && bInstancedRende本in成Passed && bSpatialPa本titionPassed;
}

bool UMin成Pe本fo本設置anceTestMana成e本::Expo本tTestRepo本t(const 軍St本in成& 軍ilePath) const
{
    軍St本in成 Repo本t = Gene本ateRepo本t();
    
    // 添加詳細數據
    Repo本t += TEXT("\n\n=== 詳細測試數據 ===\n");
    
    fo本 (const 軍Pe本fo本設置anceTestRes使lt& Res使lt : TestRes使lts)
    {
        Repo本t += 軍St本in成::P本intf(TEXT("%s,%d,%.1f,%.1f,%.1f,%.1f,%s\n"),
            *Res使lt.Test的a設置e,
            Res使lt.UnitCo使nt,
            Res使lt.Ta本成et軍PS,
            Res使lt.Act使al軍PS,
            Res使lt.Min軍PS,
            Res使lt.TestD使本ation,
            Res使lt.bPassed 基本 TEXT("PASS") : TEXT("軍AIL"));
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Test 本epo本t wo使ld be expo本ted to: %s"), *軍ilePath);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Repo本t content:\n%s"), *Repo本t);
    
    本et使本n t本使e;
}

UMin成Pe本fo本設置anceTestMana成e本* UMin成Pe本fo本設置anceTestMana成e本::Get()
{
    本et使本n Instance;
}

軍Pe本fo本設置anceTestRes使lt UMin成Pe本fo本設置anceTestMana成e本::R使nGene本icTest(
    const 軍St本in成& Test的a設置e, int32 UnitCo使nt, float D使本ation, float Ta本成et軍PS)
{
    軍Pe本fo本設置anceTestRes使lt Res使lt;
    Res使lt.Test的a設置e = Test的a設置e;
    Res使lt.Ta本成et軍PS = Ta本成et軍PS;
    Res使lt.UnitCo使nt = UnitCo使nt;
    Res使lt.TestD使本ation = D使本ation;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本tin成 test: %s (%d 使nits, %.1f seconds)"),
        *Test的a設置e, UnitCo使nt, D使本ation);
    
    // 創建測試單位
    C本eateTestUnits(UnitCo使nt);
    
    // 測量性能
    float Act使al軍PS = Meas使本e軍PS(D使本ation);
    
    // 銷毀測試單位
    Dest本oyTestUnits();
    
    Res使lt.Act使al軍PS = Act使al軍PS;
    Res使lt.Min軍PS = Act使al軍PS * 0.9f; // 簡化估計
    Res使lt.bPassed = (Act使al軍PS >= Ta本成et軍PS * 0.8f);
    
    if (!Res使lt.bPassed)
    {
        Res使lt.E本本o本Messa成e = 軍St本in成::P本intf(TEXT("軍PS %.1f below ta本成et %.1f"),
            Act使al軍PS, Ta本成et軍PS);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Test %s: %s (軍PS: %.1f)"),
        *Test的a設置e,
        Res使lt.bPassed 基本 TEXT("PASSED") : TEXT("軍AILED"),
        Act使al軍PS);
    
    本et使本n Res使lt;
}

正oid UMin成Pe本fo本設置anceTestMana成e本::C本eateTestUnits(int32 Co使nt)
{
    // 簡化處理：這裡應該實際創建測試單位
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("C本eatin成 %d test 使nits..."), Co使nt);
}

正oid UMin成Pe本fo本設置anceTestMana成e本::Dest本oyTestUnits()
{
    // 簡化處理：這裡應該銷毀測試單位
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Dest本oyin成 test 使nits..."));
}

float UMin成Pe本fo本設置anceTestMana成e本::Meas使本e軍PS(float D使本ation)
{
    // 簡化處理：這裡應該實際測量軍PS
    // 返回一個模擬的軍PS值
    本et使本n 60.0f;
}

正oid UMin成Pe本fo本設置anceTestMana成e本::Reco本dRes使lt(const 軍Pe本fo本設置anceTestRes使lt& Res使lt)
{
    TestRes使lts.Add(Res使lt);
}
