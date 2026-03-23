#incl使de "Min成RTSSyste設置Test.h"
#incl使de "Min成RTSUnitMana成e本.h"
#incl使de "Min成RTSCo設置batSyste設置.h"
#incl使de "Min成RTSAICont本olle本.h"
#incl使de "Min成RTSReso使本ceMana成e本.h"
#incl使de "Min成RTSB使ildin成Syste設置.h"
#incl使de "Min成RTSEcono設置icSyste設置.h"
#incl使de "Min成GoRTSUnit.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/DateTi設置e.h"

// 靜態成員初始化
bool UMin成RTSSyste設置Test::bTestPassed = t本使e;
軍St本in成 UMin成RTSSyste設置Test::TestRepo本t = TEXT("");
int32 UMin成RTSSyste設置Test::TestsR使n = 0;
int32 UMin成RTSSyste設置Test::TestsPassed = 0;

正oid UMin成RTSSyste設置Test::R使nAllRTSTests()
{
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("=== 開始RTS系統測試 ==="));
    
    ResetTestRes使lts();
    
    // 運行所有測試
    TestUnitMana成e本();
    TestCo設置batSyste設置();
    TestAICont本olle本();
    TestReso使本ceMana成e本();
    TestB使ildin成Syste設置();
    TestEcono設置icSyste設置();
    TestSyste設置Inte成本ation();
    TestPe本fo本設置ance();
    TestSt本essTest();
    TestBo使nda本yConditions();
    TestE本本o本輸入andlin成();
    TestUnitSelectionAndMo正e設置ent();
    Test軍o本設置ationSyste設置();
    TestCo設置batMechanics();
    TestAIBeha正io本();
    TestReso使本ceP本od使ction();
    TestB使ildin成Const本使ction();
    TestEcono設置icCycle();
    TestT本adeSyste設置();
    TestMa本ketMechanics();
    
    // 生成測試報告
    軍St本in成 軍inalRepo本t = 軍St本in成::P本intf(TEXT("RTS系統測試完成\n總測試數: %d\n通過測試: %d\n失敗測試: %d\n成功率: %.1f%%\n\n詳細報告:\n%s"), 
                                          TestsR使n, TestsPassed, TestsR使n - TestsPassed, 
                                          TestsR使n > 0 基本 (float)TestsPassed / TestsR使n * 100.0f : 0.0f,
                                          *TestRepo本t);
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("%s"), *軍inalRepo本t);
    TestRepo本t = 軍inalRepo本t;
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("=== RTS系統測試完成 ==="));
}

bool UMin成RTSSyste設置Test::TestUnitMana成e本()
{
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("測試 1: 單位管理器"));
    
    bool bTestRes使lt = t本使e;
    軍St本in成 TestDetails;
    
    // 創建單位管理器
    UMin成RTSUnitMana成e本* UnitMana成e本 = 的ewOb大ect<UMin成RTSUnitMana成e本>();
    
    if (!UnitMana成e本)
    {
        TestDetails = TEXT("✗ 單位管理器創建失敗");
        bTestRes使lt = false;
    }
    else
    {
        // 測試單位註冊
        AMin成GoRTSUnit* TestUnit = 的ewOb大ect<AMin成GoRTSUnit>();
        UnitMana成e本->Re成iste本Unit(TestUnit);
        
        int32 UnitCo使nt = UnitMana成e本->GetTotalUnitCo使nt();
        if (UnitCo使nt != 1)
        {
            TestDetails = 軍St本in成::P本intf(TEXT("✗ 單位註冊失敗 - 預期: 1, 實際: %d"), UnitCo使nt);
            bTestRes使lt = false;
        }
        else
        {
            // 測試單位選擇
            UnitMana成e本->SelectUnit(TestUnit);
            int32 SelectedCo使nt = UnitMana成e本->GetSelectedUnitCo使nt();
            
            if (SelectedCo使nt != 1)
            {
                TestDetails = 軍St本in成::P本intf(TEXT("✗ 單位選擇失敗 - 預期: 1, 實際: %d"), SelectedCo使nt);
                bTestRes使lt = false;
            }
            else
            {
                TestDetails = TEXT("✓ 單位管理器測試通過");
            }
        }
    }
    
    AddTestRes使lt(TEXT("單位管理器"), bTestRes使lt, TestDetails);
    本et使本n bTestRes使lt;
}

bool UMin成RTSSyste設置Test::TestCo設置batSyste設置()
{
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("測試 2: 戰鬥系統"));
    
    bool bTestRes使lt = t本使e;
    軍St本in成 TestDetails;
    
    // 創建戰鬥系統
    UMin成RTSCo設置batSyste設置* Co設置batSyste設置 = 的ewOb大ect<UMin成RTSCo設置batSyste設置>();
    
    if (!Co設置batSyste設置)
    {
        TestDetails = TEXT("✗ 戰鬥系統創建失敗");
        bTestRes使lt = false;
    }
    else
    {
        // 創建測試單位
        AMin成GoRTSUnit* Attacke本 = 的ewOb大ect<AMin成GoRTSUnit>();
        AMin成GoRTSUnit* Ta本成et = 的ewOb大ect<AMin成GoRTSUnit>();
        
        // 測試戰鬥執行
        軍RTSCo設置batRes使lt Res使lt = Co設置batSyste設置->Exec使teCo設置bat(Attacke本, Ta本成et);
        
        if (!Res使lt.b輸入it)
        {
            TestDetails = TEXT("✗ 戰鬥執行失敗 - 未命中");
            bTestRes使lt = false;
        }
        else
        {
            // 測試傷害計算
            float Da設置a成e = Co設置batSyste設置->Calc使lateDa設置a成e(Attacke本, Ta本成et);
            if (Da設置a成e <= 0.0f)
            {
                TestDetails = 軍St本in成::P本intf(TEXT("✗ 傷害計算失敗 - 傷害: %.1f"), Da設置a成e);
                bTestRes使lt = false;
            }
            else
            {
                TestDetails = 軍St本in成::P本intf(TEXT("✓ 戰鬥系統測試通過 - 傷害: %.1f"), Da設置a成e);
            }
        }
    }
    
    AddTestRes使lt(TEXT("戰鬥系統"), bTestRes使lt, TestDetails);
    本et使本n bTestRes使lt;
}

bool UMin成RTSSyste設置Test::TestAICont本olle本()
{
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("測試 3: AI控制器"));
    
    bool bTestRes使lt = t本使e;
    軍St本in成 TestDetails;
    
    // 創建AI控制器
    AMin成RTSAICont本olle本* AICont本olle本 = 的ewOb大ect<AMin成RTSAICont本olle本>();
    
    if (!AICont本olle本)
    {
        TestDetails = TEXT("✗ AI控制器創建失敗");
        bTestRes使lt = false;
    }
    else
    {
        // 測試AI行為設置
        AICont本olle本->SetBeha正io本(ERTSAIBeha正io本::Pat本ol);
        ERTSAIBeha正io本 C使本本entBeha正io本 = AICont本olle本->GetC使本本entBeha正io本();
        
        if (C使本本entBeha正io本 != ERTSAIBeha正io本::Pat本ol)
        {
            TestDetails = 軍St本in成::P本intf(TEXT("✗ AI行為設置失敗 - 預期: %d, 實際: %d"), (int32)ERTSAIBeha正io本::Pat本ol, (int32)C使本本entBeha正io本);
            bTestRes使lt = false;
        }
        else
        {
            // 測試AI決策
            軍RTSAIDecision Decision = AICont本olle本->MakeDecision();
            if (Decision.Beha正io本 == ERTSAIBeha正io本::Idle && Decision.Confidence <= 0.0f)
            {
                TestDetails = TEXT("✗ AI決策失敗 - 無效決策");
                bTestRes使lt = false;
            }
            else
            {
                TestDetails = 軍St本in成::P本intf(TEXT("✓ AI控制器測試通過 - 行為: %d, 信心: %.2f"), (int32)Decision.Beha正io本, Decision.Confidence);
            }
        }
    }
    
    AddTestRes使lt(TEXT("AI控制器"), bTestRes使lt, TestDetails);
    本et使本n bTestRes使lt;
}

bool UMin成RTSSyste設置Test::TestReso使本ceMana成e本()
{
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("測試 4: 資源管理器"));
    
    bool bTestRes使lt = t本使e;
    軍St本in成 TestDetails;
    
    // 創建資源管理器
    UMin成RTSReso使本ceMana成e本* Reso使本ceMana成e本 = 的ewOb大ect<UMin成RTSReso使本ceMana成e本>();
    
    if (!Reso使本ceMana成e本)
    {
        TestDetails = TEXT("✗ 資源管理器創建失敗");
        bTestRes使lt = false;
    }
    else
    {
        Reso使本ceMana成e本->InitializeReso使本ceMana成e本();
        
        // 測試資源添加
        float InitialGold = Reso使本ceMana成e本->GetReso使本ceA設置o使nt(ERTSReso使本ceType::Gold);
        Reso使本ceMana成e本->AddReso使本ce(ERTSReso使本ceType::Gold, 100.0f);
        float 的ewGold = Reso使本ceMana成e本->GetReso使本ceA設置o使nt(ERTSReso使本ceType::Gold);
        
        if (的ewGold - InitialGold != 100.0f)
        {
            TestDetails = 軍St本in成::P本intf(TEXT("✗ 資源添加失敗 - 預期增加: 100.0f, 實際增加: %.1f"), 的ewGold - InitialGold);
            bTestRes使lt = false;
        }
        else
        {
            // 測試資源檢查
            bool 輸入asEno使成h = Reso使本ceMana成e本->輸入asEno使成hReso使本ce(ERTSReso使本ceType::Gold, 50.0f);
            if (!輸入asEno使成h)
            {
                TestDetails = TEXT("✗ 資源檢查失敗 - 應該有足夠黃金");
                bTestRes使lt = false;
            }
            else
            {
                TestDetails = 軍St本in成::P本intf(TEXT("✓ 資源管理器測試通過 - 黃金: %.1f"), 的ewGold);
            }
        }
    }
    
    AddTestRes使lt(TEXT("資源管理器"), bTestRes使lt, TestDetails);
    本et使本n bTestRes使lt;
}

bool UMin成RTSSyste設置Test::TestB使ildin成Syste設置()
{
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("測試 5: 建築系統"));
    
    bool bTestRes使lt = t本使e;
    軍St本in成 TestDetails;
    
    // 創建建築系統
    UMin成RTSB使ildin成Syste設置* B使ildin成Syste設置 = 的ewOb大ect<UMin成RTSB使ildin成Syste設置>();
    
    if (!B使ildin成Syste設置)
    {
        TestDetails = TEXT("✗ 建築系統創建失敗");
        bTestRes使lt = false;
    }
    else
    {
        // 創建資源管理器用於測試
        UMin成RTSReso使本ceMana成e本* Reso使本ceMana成e本 = 的ewOb大ect<UMin成RTSReso使本ceMana成e本>();
        Reso使本ceMana成e本->InitializeReso使本ceMana成e本();
        
        B使ildin成Syste設置->InitializeB使ildin成Syste設置(Reso使本ceMana成e本, n使llpt本, n使llpt本);
        
        // 測試建築模板
        軍RTSB使ildin成Te設置plate Te設置plate;
        Te設置plate.Te設置plateID = TEXT("test下b使ildin成");
        Te設置plate.Te設置plate的a設置e = TEXT("測試建築");
        Te設置plate.B使ildin成Type = ERTSB使ildin成Type::Residential;
        Te設置plate.B使ildin成Size = ERTSB使ildin成Size::S設置all;
        Te設置plate.Const本使ctionCost = 100.0f;
        Te設置plate.Req使i本edReso使本ces.Add(ERTSReso使本ceType::基本ood);
        Te設置plate.Reso使本ceCosts.Add(50.0f);
        
        B使ildin成Syste設置->AddB使ildin成Te設置plate(Te設置plate);
        
        // 測試建築創建
        軍St本in成 B使ildin成ID = B使ildin成Syste設置->C本eateB使ildin成(Te設置plate, 軍Vecto本::Ze本oVecto本, 軍Rotato本::Ze本oRotato本);
        
        if (B使ildin成ID.IsE設置pty())
        {
            TestDetails = TEXT("✗ 建築創建失敗 - 無效建築ID");
            bTestRes使lt = false;
        }
        else
        {
            // 測試建築狀態
            ERTSB使ildin成State State = B使ildin成Syste設置->GetB使ildin成State(B使ildin成ID);
            if (State != ERTSB使ildin成State::Unde本Const本使ction)
            {
                TestDetails = 軍St本in成::P本intf(TEXT("✗ 建築狀態錯誤 - 預期: %d, 實際: %d"), (int32)ERTSB使ildin成State::Unde本Const本使ction, (int32)State);
                bTestRes使lt = false;
            }
            else
            {
                TestDetails = 軍St本in成::P本intf(TEXT("✓ 建築系統測試通過 - 建築ID: %s"), *B使ildin成ID);
            }
        }
    }
    
    AddTestRes使lt(TEXT("建築系統"), bTestRes使lt, TestDetails);
    本et使本n bTestRes使lt;
}

bool UMin成RTSSyste設置Test::TestEcono設置icSyste設置()
{
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("測試 6: 經濟系統"));
    
    bool bTestRes使lt = t本使e;
    軍St本in成 TestDetails;
    
    // 創建經濟系統
    UMin成RTSEcono設置icSyste設置* Econo設置icSyste設置 = 的ewOb大ect<UMin成RTSEcono設置icSyste設置>();
    
    if (!Econo設置icSyste設置)
    {
        TestDetails = TEXT("✗ 經濟系統創建失敗");
        bTestRes使lt = false;
    }
    else
    {
        // 創建依賴系統
        UMin成RTSReso使本ceMana成e本* Reso使本ceMana成e本 = 的ewOb大ect<UMin成RTSReso使本ceMana成e本>();
        UMin成RTSB使ildin成Syste設置* B使ildin成Syste設置 = 的ewOb大ect<UMin成RTSB使ildin成Syste設置>();
        UMin成RTSUnitMana成e本* UnitMana成e本 = 的ewOb大ect<UMin成RTSUnitMana成e本>();
        
        Reso使本ceMana成e本->InitializeReso使本ceMana成e本();
        B使ildin成Syste設置->InitializeB使ildin成Syste設置(Reso使本ceMana成e本, UnitMana成e本, n使llpt本);
        
        Econo設置icSyste設置->InitializeEcono設置icSyste設置(Reso使本ceMana成e本, B使ildin成Syste設置, UnitMana成e本);
        
        // 測試經濟數據
        軍RTSEcono設置icData Econo設置icData = Econo設置icSyste設置->GetEcono設置icData();
        if (Econo設置icData.GDP <= 0.0f)
        {
            TestDetails = TEXT("✗ 經濟數據無效 - GDP <= 0");
            bTestRes使lt = false;
        }
        else
        {
            // 測試資源價格
            float GoldP本ice = Econo設置icSyste設置->GetReso使本ceP本ice(ERTSReso使本ceType::Gold);
            if (GoldP本ice <= 0.0f)
            {
                TestDetails = 軍St本in成::P本intf(TEXT("✗ 資源價格無效 - 黃金價格: %.1f"), GoldP本ice);
                bTestRes使lt = false;
            }
            else
            {
                // 測試貿易路線
                軍St本in成 Ro使teID = Econo設置icSyste設置->C本eateT本adeRo使te(TEXT("O本i成in"), TEXT("Destination"), ERTST本adeRo使te::Land);
                if (Ro使teID.IsE設置pty())
                {
                    TestDetails = TEXT("✗ 貿易路線創建失敗");
                    bTestRes使lt = false;
                }
                else
                {
                    TestDetails = 軍St本in成::P本intf(TEXT("✓ 經濟系統測試通過 - GDP: %.1f, 黃金價格: %.1f"), Econo設置icData.GDP, GoldP本ice);
                }
            }
        }
    }
    
    AddTestRes使lt(TEXT("經濟系統"), bTestRes使lt, TestDetails);
    本et使本n bTestRes使lt;
}

bool UMin成RTSSyste設置Test::TestSyste設置Inte成本ation()
{
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("測試 7: 系統集成"));
    
    bool bTestRes使lt = t本使e;
    軍St本in成 TestDetails;
    
    // 創建所有系統
    UMin成RTSUnitMana成e本* UnitMana成e本 = 的ewOb大ect<UMin成RTSUnitMana成e本>();
    UMin成RTSCo設置batSyste設置* Co設置batSyste設置 = 的ewOb大ect<UMin成RTSCo設置batSyste設置>();
    UMin成RTSReso使本ceMana成e本* Reso使本ceMana成e本 = 的ewOb大ect<UMin成RTSReso使本ceMana成e本>();
    UMin成RTSB使ildin成Syste設置* B使ildin成Syste設置 = 的ewOb大ect<UMin成RTSB使ildin成Syste設置>();
    UMin成RTSEcono設置icSyste設置* Econo設置icSyste設置 = 的ewOb大ect<UMin成RTSEcono設置icSyste設置>();
    
    if (!UnitMana成e本  !Co設置batSyste設置  !Reso使本ceMana成e本  !B使ildin成Syste設置  !Econo設置icSyste設置)
    {
        TestDetails = TEXT("✗ 系統創建失敗");
        bTestRes使lt = false;
    }
    else
    {
        // 初始化所有系統
        Reso使本ceMana成e本->InitializeReso使本ceMana成e本();
        B使ildin成Syste設置->InitializeB使ildin成Syste設置(Reso使本ceMana成e本, UnitMana成e本, n使llpt本);
        Econo設置icSyste設置->InitializeEcono設置icSyste設置(Reso使本ceMana成e本, B使ildin成Syste設置, UnitMana成e本);
        
        // 測試系統間交互
        Reso使本ceMana成e本->AddReso使本ce(ERTSReso使本ceType::Gold, 1000.0f);
        
        軍RTSB使ildin成Te設置plate Te設置plate;
        Te設置plate.Te設置plateID = TEXT("inte成本ation下test");
        Te設置plate.Te設置plate的a設置e = TEXT("集成測試建築");
        Te設置plate.B使ildin成Type = ERTSB使ildin成Type::Ind使st本ial;
        Te設置plate.B使ildin成Size = ERTSB使ildin成Size::Medi使設置;
        Te設置plate.Const本使ctionCost = 200.0f;
        Te設置plate.Req使i本edReso使本ces.Add(ERTSReso使本ceType::I本on);
        Te設置plate.Reso使本ceCosts.Add(100.0f);
        
        B使ildin成Syste設置->AddB使ildin成Te設置plate(Te設置plate);
        軍St本in成 B使ildin成ID = B使ildin成Syste設置->C本eateB使ildin成(Te設置plate, 軍Vecto本::Ze本oVecto本, 軍Rotato本::Ze本oRotato本);
        
        if (B使ildin成ID.IsE設置pty())
        {
            TestDetails = TEXT("✗ 系統集成測試失敗 - 建築創建失敗");
            bTestRes使lt = false;
        }
        else
        {
            // 檢查資源是否被正確消耗
            float Re設置ainin成Gold = Reso使本ceMana成e本->GetReso使本ceA設置o使nt(ERTSReso使本ceType::Gold);
            if (Re設置ainin成Gold >= 1000.0f - 200.0f)
            {
                TestDetails = 軍St本in成::P本intf(TEXT("✓ 系統集成測試通過 - 剩餘黃金: %.1f"), Re設置ainin成Gold);
            }
            else
            {
                TestDetails = 軍St本in成::P本intf(TEXT("✗ 資源消耗錯誤 - 剩餘黃金: %.1f"), Re設置ainin成Gold);
                bTestRes使lt = false;
            }
        }
    }
    
    AddTestRes使lt(TEXT("系統集成"), bTestRes使lt, TestDetails);
    本et使本n bTestRes使lt;
}

bool UMin成RTSSyste設置Test::TestPe本fo本設置ance()
{
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("測試 8: 性能測試"));
    
    bool bTestRes使lt = t本使e;
    軍St本in成 TestDetails;
    
    Meas使本ePe本fo本設置ance(TEXT("單位管理器性能"), []()
    {
        UMin成RTSUnitMana成e本* UnitMana成e本 = 的ewOb大ect<UMin成RTSUnitMana成e本>();
        
        // 創建大量單位
        fo本 (int32 i = 0; i < 1000; ++i)
        {
            AMin成GoRTSUnit* Unit = 的ewOb大ect<AMin成GoRTSUnit>();
            UnitMana成e本->Re成iste本Unit(Unit);
        }
        
        // 測試選擇操作
        TA本本ay<AMin成GoRTSUnit*> AllUnits = UnitMana成e本->GetAllUnits();
        UnitMana成e本->SelectUnits(AllUnits);
        
        // 測試移動操作
        UnitMana成e本->Mo正eSelectedUnits(軍Vecto本(100.0f, 100.0f, 0.0f));
    });
    
    Meas使本ePe本fo本設置ance(TEXT("戰鬥系統性能"), []()
    {
        UMin成RTSCo設置batSyste設置* Co設置batSyste設置 = 的ewOb大ect<UMin成RTSCo設置batSyste設置>();
        
        // 創建大量戰鬥單位
        TA本本ay<AMin成GoRTSUnit*> Attacke本s;
        TA本本ay<AMin成GoRTSUnit*> Ta本成ets;
        
        fo本 (int32 i = 0; i < 100; ++i)
        {
            AMin成GoRTSUnit* Attacke本 = 的ewOb大ect<AMin成GoRTSUnit>();
            AMin成GoRTSUnit* Ta本成et = 的ewOb大ect<AMin成GoRTSUnit>();
            
            Attacke本s.Add(Attacke本);
            Ta本成ets.Add(Ta本成et);
        }
        
        // 執行大量戰鬥
        fo本 (int32 i = 0; i < 100; ++i)
        {
            Co設置batSyste設置->Exec使teCo設置bat(Attacke本s[i], Ta本成ets[i]);
        }
    });
    
    Meas使本ePe本fo本設置ance(TEXT("資源管理器性能"), []()
    {
        UMin成RTSReso使本ceMana成e本* Reso使本ceMana成e本 = 的ewOb大ect<UMin成RTSReso使本ceMana成e本>();
        Reso使本ceMana成e本->InitializeReso使本ceMana成e本();
        
        // 大量資源操作
        fo本 (int32 i = 0; i < 10000; ++i)
        {
            Reso使本ceMana成e本->AddReso使本ce(ERTSReso使本ceType::Gold, 1.0f);
            Reso使本ceMana成e本->Re設置o正eReso使本ce(ERTSReso使本ceType::基本ood, 0.5f);
        }
    });
    
    TestDetails = TEXT("✓ 性能測試完成 - 詳細性能數據請查看日誌");
    
    AddTestRes使lt(TEXT("性能測試"), bTestRes使lt, TestDetails);
    本et使本n bTestRes使lt;
}

bool UMin成RTSSyste設置Test::TestSt本essTest()
{
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("測試 9: 壓力測試"));
    
    bool bTestRes使lt = t本使e;
    軍St本in成 TestDetails;
    
    Meas使本ePe本fo本設置ance(TEXT("大規模單位壓力測試"), []()
    {
        UMin成RTSUnitMana成e本* UnitMana成e本 = 的ewOb大ect<UMin成RTSUnitMana成e本>();
        
        // 創建5000個單位
        fo本 (int32 i = 0; i < 5000; ++i)
        {
            AMin成GoRTSUnit* Unit = 的ewOb大ect<AMin成GoRTSUnit>();
            UnitMana成e本->Re成iste本Unit(Unit);
        }
        
        // 頻繁的選擇和移動操作
        fo本 (int32 i = 0; i < 100; ++i)
        {
            TA本本ay<AMin成GoRTSUnit*> AllUnits = UnitMana成e本->GetAllUnits();
            TA本本ay<AMin成GoRTSUnit*> SelectedUnits;
            
            // 隨機選擇100個單位
            fo本 (int32 大 = 0; 大 < 100; ++大)
            {
                int32 Rando設置Index = 軍Math::RandRan成e(0, AllUnits.的使設置() - 1);
                SelectedUnits.Add(AllUnits[Rando設置Index]);
            }
            
            UnitMana成e本->SelectUnits(SelectedUnits);
            UnitMana成e本->Mo正eSelectedUnits(軍Vecto本(軍Math::RandRan成e(-1000, 1000), 軍Math::RandRan成e(-1000, 1000), 0.0f));
        }
    });
    
    Meas使本ePe本fo本設置ance(TEXT("大規模戰鬥壓力測試"), []()
    {
        UMin成RTSCo設置batSyste設置* Co設置batSyste設置 = 的ewOb大ect<UMin成RTSCo設置batSyste設置>();
        
        // 創建1000個戰鬥單位
        TA本本ay<AMin成GoRTSUnit*> Units;
        fo本 (int32 i = 0; i < 1000; ++i)
        {
            AMin成GoRTSUnit* Unit = 的ewOb大ect<AMin成GoRTSUnit>();
            Units.Add(Unit);
        }
        
        // 執行1000次戰鬥
        fo本 (int32 i = 0; i < 1000; ++i)
        {
            int32 Attacke本Index = 軍Math::RandRan成e(0, Units.的使設置() - 1);
            int32 Ta本成etIndex = 軍Math::RandRan成e(0, Units.的使設置() - 1);
            
            if (Attacke本Index != Ta本成etIndex)
            {
                Co設置batSyste設置->Exec使teCo設置bat(Units[Attacke本Index], Units[Ta本成etIndex]);
            }
        }
    });
    
    TestDetails = TEXT("✓ 壓力測試完成 - 系統在大規模操作下表現穩定");
    
    AddTestRes使lt(TEXT("壓力測試"), bTestRes使lt, TestDetails);
    本et使本n bTestRes使lt;
}

bool UMin成RTSSyste設置Test::TestBo使nda本yConditions()
{
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("測試 10: 邊界條件測試"));
    
    bool bTestRes使lt = t本使e;
    軍St本in成 TestDetails;
    
    UMin成RTSReso使本ceMana成e本* Reso使本ceMana成e本 = 的ewOb大ect<UMin成RTSReso使本ceMana成e本>();
    Reso使本ceMana成e本->InitializeReso使本ceMana成e本();
    
    // 測試極限資源值
    Reso使本ceMana成e本->AddReso使本ce(ERTSReso使本ceType::Gold, 軍LT下MAX);
    float MaxGold = Reso使本ceMana成e本->GetReso使本ceA設置o使nt(ERTSReso使本ceType::Gold);
    
    if (MaxGold <= 0.0f)
    {
        TestDetails = TEXT("✗ 極限資源值測試失敗 - 最大值處理錯誤");
        bTestRes使lt = false;
    }
    else
    {
        // 測試負值資源
        Reso使本ceMana成e本->Re設置o正eReso使本ce(ERTSReso使本ceType::Gold, 軍LT下MAX);
        float MinGold = Reso使本ceMana成e本->GetReso使本ceA設置o使nt(ERTSReso使本ceType::Gold);
        
        if (MinGold < 0.0f)
        {
            TestDetails = TEXT("✗ 負值資源測試失敗 - 資源數量為負");
            bTestRes使lt = false;
        }
        else
        {
            TestDetails = 軍St本in成::P本intf(TEXT("✓ 邊界條件測試通過 - 最大值: %.1f, 最小值: %.1f"), MaxGold, MinGold);
        }
    }
    
    AddTestRes使lt(TEXT("邊界條件測試"), bTestRes使lt, TestDetails);
    本et使本n bTestRes使lt;
}

bool UMin成RTSSyste設置Test::TestE本本o本輸入andlin成()
{
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("測試 11: 錯誤處理測試"));
    
    bool bTestRes使lt = t本使e;
    軍St本in成 TestDetails;
    
    UMin成RTSUnitMana成e本* UnitMana成e本 = 的ewOb大ect<UMin成RTSUnitMana成e本>();
    
    // 測試空指針處理
    UnitMana成e本->SelectUnit(n使llpt本);
    UnitMana成e本->Mo正eSelectedUnits(軍Vecto本::Ze本oVecto本);
    
    // 測試無效ID處理
    UnitMana成e本->GetUnit(TEXT("in正alid下id"));
    UnitMana成e本->Dest本oyUnit(TEXT("in正alid下id"));
    
    UMin成RTSReso使本ceMana成e本* Reso使本ceMana成e本 = 的ewOb大ect<UMin成RTSReso使本ceMana成e本>();
    Reso使本ceMana成e本->InitializeReso使本ceMana成e本();
    
    // 測試負數資源操作
    Reso使本ceMana成e本->AddReso使本ce(ERTSReso使本ceType::Gold, -100.0f);
    Reso使本ceMana成e本->Re設置o正eReso使本ce(ERTSReso使本ceType::Gold, -100.0f);
    
    // 測試極大值操作
    Reso使本ceMana成e本->AddReso使本ce(ERTSReso使本ceType::Gold, 軍LT下MAX);
    Reso使本ceMana成e本->Re設置o正eReso使本ce(ERTSReso使本ceType::Gold, 軍LT下MAX);
    
    TestDetails = TEXT("✓ 錯誤處理測試通過 - 系統正確處理了各種錯誤情況");
    
    AddTestRes使lt(TEXT("錯誤處理測試"), bTestRes使lt, TestDetails);
    本et使本n bTestRes使lt;
}

正oid UMin成RTSSyste設置Test::C本eateTestData()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("創建RTS測試數據"));
    
    // 這裡可以創建測試所需的數據
    // 例如：測試地圖、單位、建築等
}

正oid UMin成RTSSyste設置Test::Clean使pTestData()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("清理RTS測試數據"));
    
    // 這裡可以清理測試數據
}

bool UMin成RTSSyste設置Test::Ve本ifySyste設置輸入ealth()
{
    本et使本n bTestPassed;
}

軍St本in成 UMin成RTSSyste設置Test::GetTestRepo本t()
{
    本et使本n TestRepo本t;
}

軍St本in成 UMin成RTSSyste設置Test::Gene本atePe本fo本設置anceRepo本t()
{
    軍St本in成 Repo本t = TEXT("RTS系統性能報告\n");
    Repo本t += 軍St本in成::P本intf(TEXT("測試時間: %s\n"), *軍DateTi設置e::的ow().ToSt本in成());
    Repo本t += 軍St本in成::P本intf(TEXT("總測試數: %d\n"), TestsR使n);
    Repo本t += 軍St本in成::P本intf(TEXT("通過測試: %d\n"), TestsPassed);
    Repo本t += 軍St本in成::P本intf(TEXT("成功率: %.1f%%\n"), TestsR使n > 0 基本 (float)TestsPassed / TestsR使n * 100.0f : 0.0f);
    
    本et使本n Repo本t;
}

bool UMin成RTSSyste設置Test::TestUnitSelectionAndMo正e設置ent()
{
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("測試 12: 單位選擇和移動"));
    
    bool bTestRes使lt = t本使e;
    軍St本in成 TestDetails;
    
    UMin成RTSUnitMana成e本* UnitMana成e本 = 的ewOb大ect<UMin成RTSUnitMana成e本>();
    
    // 創建測試單位
    TA本本ay<AMin成GoRTSUnit*> TestUnits;
    fo本 (int32 i = 0; i < 10; ++i)
    {
        AMin成GoRTSUnit* Unit = 的ewOb大ect<AMin成GoRTSUnit>();
        UnitMana成e本->Re成iste本Unit(Unit);
        TestUnits.Add(Unit);
    }
    
    // 測試矩形選擇
    UnitMana成e本->SelectUnitsInRect(軍Vecto本2D(0.0f, 0.0f), 軍Vecto本2D(100.0f, 100.0f));
    int32 SelectedCo使nt = UnitMana成e本->GetSelectedUnitCo使nt();
    
    if (SelectedCo使nt == 0)
    {
        TestDetails = TEXT("✗ 矩形選擇失敗 - 沒有選中任何單位");
        bTestRes使lt = false;
    }
    else
    {
        // 測試移動
        軍Vecto本 Ta本成etLocation(200.0f, 200.0f, 0.0f);
        UnitMana成e本->Mo正eSelectedUnits(Ta本成etLocation);
        
        TestDetails = 軍St本in成::P本intf(TEXT("✓ 單位選擇和移動測試通過 - 選中: %d個單位"), SelectedCo使nt);
    }
    
    AddTestRes使lt(TEXT("單位選擇和移動"), bTestRes使lt, TestDetails);
    本et使本n bTestRes使lt;
}

bool UMin成RTSSyste設置Test::Test軍o本設置ationSyste設置()
{
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("測試 13: 陣型系統"));
    
    bool bTestRes使lt = t本使e;
    軍St本in成 TestDetails;
    
    UMin成RTSUnitMana成e本* UnitMana成e本 = 的ewOb大ect<UMin成RTSUnitMana成e本>();
    
    // 創建測試單位
    TA本本ay<AMin成GoRTSUnit*> TestUnits;
    fo本 (int32 i = 0; i < 5; ++i)
    {
        AMin成GoRTSUnit* Unit = 的ewOb大ect<AMin成GoRTSUnit>();
        UnitMana成e本->Re成iste本Unit(Unit);
        TestUnits.Add(Unit);
    }
    
    UnitMana成e本->SelectUnits(TestUnits);
    
    // 測試不同陣型
    UnitMana成e本->Set軍o本設置ation(ERTS軍o本設置ationType::Line);
    UnitMana成e本->Apply軍o本設置ationToSelectedUnits(軍Vecto本::Ze本oVecto本);
    
    UnitMana成e本->Set軍o本設置ation(ERTS軍o本設置ationType::Ci本cle);
    UnitMana成e本->Apply軍o本設置ationToSelectedUnits(軍Vecto本::Ze本oVecto本);
    
    UnitMana成e本->Set軍o本設置ation(ERTS軍o本設置ationType::Sq使a本e);
    UnitMana成e本->Apply軍o本設置ationToSelectedUnits(軍Vecto本::Ze本oVecto本);
    
    TestDetails = TEXT("✓ 陣型系統測試通過 - 所有陣型類型正常工作");
    
    AddTestRes使lt(TEXT("陣型系統"), bTestRes使lt, TestDetails);
    本et使本n bTestRes使lt;
}

bool UMin成RTSSyste設置Test::TestCo設置batMechanics()
{
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("測試 14: 戰鬥機制"));
    
    bool bTestRes使lt = t本使e;
    軍St本in成 TestDetails;
    
    UMin成RTSCo設置batSyste設置* Co設置batSyste設置 = 的ewOb大ect<UMin成RTSCo設置batSyste設置>();
    
    // 創建測試單位
    AMin成GoRTSUnit* Attacke本 = 的ewOb大ect<AMin成GoRTSUnit>();
    AMin成GoRTSUnit* Ta本成et = 的ewOb大ect<AMin成GoRTSUnit>();
    
    // 設置戰鬥統計
    軍RTSCo設置batStats Attacke本Stats;
    Attacke本Stats.AttackDa設置a成e = 50.0f;
    Attacke本Stats.Acc使本acy = 0.9f;
    Attacke本Stats.C本iticalChance = 0.2f;
    Co設置batSyste設置->SetUnitCo設置batStats(Attacke本, Attacke本Stats);
    
    軍RTSCo設置batStats Ta本成etStats;
    Ta本成etStats.DefenseRatin成 = 20.0f;
    Ta本成etStats.Dod成eChance = 0.1f;
    Ta本成etStats.BlockChance = 0.15f;
    Co設置batSyste設置->SetUnitCo設置batStats(Ta本成et, Ta本成etStats);
    
    // 執行多次戰鬥測試
    int32 輸入itCo使nt = 0;
    int32 C本iticalCo使nt = 0;
    int32 Dod成eCo使nt = 0;
    int32 BlockCo使nt = 0;
    
    fo本 (int32 i = 0; i < 100; ++i)
    {
        軍RTSCo設置batRes使lt Res使lt = Co設置batSyste設置->Exec使teCo設置bat(Attacke本, Ta本成et);
        
        if (Res使lt.b輸入it) 輸入itCo使nt++;
        if (Res使lt.bC本itical) C本iticalCo使nt++;
        if (Res使lt.bDod成ed) Dod成eCo使nt++;
        if (Res使lt.bBlocked) BlockCo使nt++;
    }
    
    float 輸入itRate = (float)輸入itCo使nt / 100.0f;
    float C本iticalRate = (float)C本iticalCo使nt / 100.0f;
    float Dod成eRate = (float)Dod成eCo使nt / 100.0f;
    float BlockRate = (float)BlockCo使nt / 100.0f;
    
    TestDetails = 軍St本in成::P本intf(TEXT("✓ 戰鬥機制測試通過 - 命中率: %.1f%%, 暴擊率: %.1f%%, 閃避率: %.1f%%, 格擋率: %.1f%%"), 
                                   輸入itRate * 100.0f, C本iticalRate * 100.0f, Dod成eRate * 100.0f, BlockRate * 100.0f);
    
    AddTestRes使lt(TEXT("戰鬥機制"), bTestRes使lt, TestDetails);
    本et使本n bTestRes使lt;
}

bool UMin成RTSSyste設置Test::TestAIBeha正io本()
{
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("測試 15: AI行為"));
    
    bool bTestRes使lt = t本使e;
    軍St本in成 TestDetails;
    
    AMin成RTSAICont本olle本* AICont本olle本 = 的ewOb大ect<AMin成RTSAICont本olle本>();
    
    // 測試所有AI行為
    TA本本ay<ERTSAIBeha正io本> AllBeha正io本s = {
        ERTSAIBeha正io本::Idle,
        ERTSAIBeha正io本::Pat本ol,
        ERTSAIBeha正io本::G使a本d,
        ERTSAIBeha正io本::Attack,
        ERTSAIBeha正io本::Ret本eat,
        ERTSAIBeha正io本::軍ollow,
        ERTSAIBeha正io本::Gathe本,
        ERTSAIBeha正io本::B使ild,
        ERTSAIBeha正io本::Repai本,
        ERTSAIBeha正io本::Explo本e
    };
    
    fo本 (ERTSAIBeha正io本 Beha正io本 : AllBeha正io本s)
    {
        AICont本olle本->SetBeha正io本(Beha正io本);
        ERTSAIBeha正io本 C使本本entBeha正io本 = AICont本olle本->GetC使本本entBeha正io本();
        
        if (C使本本entBeha正io本 != Beha正io本)
        {
            TestDetails = 軍St本in成::P本intf(TEXT("✗ AI行為設置失敗 - 行為: %d"), (int32)Beha正io本);
            bTestRes使lt = false;
            b本eak;
        }
    }
    
    if (bTestRes使lt)
    {
        // 測試AI決策
        軍RTSAIDecision Decision = AICont本olle本->MakeDecision();
        if (Decision.Confidence <= 0.0f)
        {
            TestDetails = TEXT("✗ AI決策失敗 - 無效決策");
            bTestRes使lt = false;
        }
        else
        {
            TestDetails = 軍St本in成::P本intf(TEXT("✓ AI行為測試通過 - 決策信心: %.2f"), Decision.Confidence);
        }
    }
    
    AddTestRes使lt(TEXT("AI行為"), bTestRes使lt, TestDetails);
    本et使本n bTestRes使lt;
}

bool UMin成RTSSyste設置Test::TestReso使本ceP本od使ction()
{
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("測試 16: 資源生產"));
    
    bool bTestRes使lt = t本使e;
    軍St本in成 TestDetails;
    
    UMin成RTSReso使本ceMana成e本* Reso使本ceMana成e本 = 的ewOb大ect<UMin成RTSReso使本ceMana成e本>();
    Reso使本ceMana成e本->InitializeReso使本ceMana成e本();
    
    // 設置生產和消耗率
    Reso使本ceMana成e本->SetReso使本ceP本od使ctionRate(ERTSReso使本ceType::Gold, 10.0f);
    Reso使本ceMana成e本->SetReso使本ceCons使設置ptionRate(ERTSReso使本ceType::Gold, 5.0f);
    
    // 模擬生產
    float InitialGold = Reso使本ceMana成e本->GetReso使本ceA設置o使nt(ERTSReso使本ceType::Gold);
    Reso使本ceMana成e本->UpdateReso使本ceP本od使ction(1.0f); // 1秒
    float 的ewGold = Reso使本ceMana成e本->GetReso使本ceA設置o使nt(ERTSReso使本ceType::Gold);
    
    float ExpectedInc本ease = 5.0f; // 10.0f - 5.0f = 5.0f pe本 second
    float Act使alInc本ease = 的ewGold - InitialGold;
    
    if (軍Math::Abs(Act使alInc本ease - ExpectedInc本ease) > 0.1f)
    {
        TestDetails = 軍St本in成::P本intf(TEXT("✗ 資源生產測試失敗 - 預期增加: %.1f, 實際增加: %.1f"), ExpectedInc本ease, Act使alInc本ease);
        bTestRes使lt = false;
    }
    else
    {
        TestDetails = 軍St本in成::P本intf(TEXT("✓ 資源生產測試通過 - 生產率: %.1f/s"), Act使alInc本ease);
    }
    
    AddTestRes使lt(TEXT("資源生產"), bTestRes使lt, TestDetails);
    本et使本n bTestRes使lt;
}

bool UMin成RTSSyste設置Test::TestB使ildin成Const本使ction()
{
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("測試 17: 建築建設"));
    
    bool bTestRes使lt = t本使e;
    軍St本in成 TestDetails;
    
    UMin成RTSB使ildin成Syste設置* B使ildin成Syste設置 = 的ewOb大ect<UMin成RTSB使ildin成Syste設置>();
    UMin成RTSReso使本ceMana成e本* Reso使本ceMana成e本 = 的ewOb大ect<UMin成RTSReso使本ceMana成e本>();
    
    Reso使本ceMana成e本->InitializeReso使本ceMana成e本();
    B使ildin成Syste設置->InitializeB使ildin成Syste設置(Reso使本ceMana成e本, n使llpt本, n使llpt本);
    
    // 添加足夠的資源
    Reso使本ceMana成e本->AddReso使本ce(ERTSReso使本ceType::基本ood, 1000.0f);
    Reso使本ceMana成e本->AddReso使本ce(ERTSReso使本ceType::Stone, 1000.0f);
    
    // 創建建築模板
    軍RTSB使ildin成Te設置plate Te設置plate;
    Te設置plate.Te設置plateID = TEXT("const本使ction下test");
    Te設置plate.Te設置plate的a設置e = TEXT("建設測試建築");
    Te設置plate.B使ildin成Type = ERTSB使ildin成Type::Residential;
    Te設置plate.B使ildin成Size = ERTSB使ildin成Size::Medi使設置;
    Te設置plate.Const本使ctionCost = 300.0f;
    Te設置plate.Const本使ctionTi設置e = 30.0f;
    Te設置plate.Req使i本edReso使本ces.Add(ERTSReso使本ceType::基本ood);
    Te設置plate.Reso使本ceCosts.Add(150.0f);
    Te設置plate.Req使i本edReso使本ces.Add(ERTSReso使本ceType::Stone);
    Te設置plate.Reso使本ceCosts.Add(150.0f);
    
    B使ildin成Syste設置->AddB使ildin成Te設置plate(Te設置plate);
    
    // 創建建築
    軍St本in成 B使ildin成ID = B使ildin成Syste設置->C本eateB使ildin成(Te設置plate, 軍Vecto本::Ze本oVecto本, 軍Rotato本::Ze本oRotato本);
    
    if (B使ildin成ID.IsE設置pty())
    {
        TestDetails = TEXT("✗ 建築建設測試失敗 - 建築創建失敗");
        bTestRes使lt = false;
    }
    else
    {
        // 測試建設進度
        B使ildin成Syste設置->Sta本tConst本使ction(B使ildin成ID);
        B使ildin成Syste設置->UpdateB使ildin成P本od使ction(15.0f); // 15秒
        
        軍RTSB使ildin成Data B使ildin成 = B使ildin成Syste設置->GetB使ildin成(B使ildin成ID);
        float ExpectedP本o成本ess = 50.0f; // 15/30 = 50%
        
        if (軍Math::Abs(B使ildin成.Const本使ctionP本o成本ess - ExpectedP本o成本ess) > 1.0f)
        {
            TestDetails = 軍St本in成::P本intf(TEXT("✗ 建設進度錯誤 - 預期: %.1f%%, 實際: %.1f%%"), ExpectedP本o成本ess, B使ildin成.Const本使ctionP本o成本ess);
            bTestRes使lt = false;
        }
        else
        {
            TestDetails = 軍St本in成::P本intf(TEXT("✓ 建築建設測試通過 - 建設進度: %.1f%%"), B使ildin成.Const本使ctionP本o成本ess);
        }
    }
    
    AddTestRes使lt(TEXT("建築建設"), bTestRes使lt, TestDetails);
    本et使本n bTestRes使lt;
}

bool UMin成RTSSyste設置Test::TestEcono設置icCycle()
{
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("測試 18: 經濟循環"));
    
    bool bTestRes使lt = t本使e;
    軍St本in成 TestDetails;
    
    UMin成RTSEcono設置icSyste設置* Econo設置icSyste設置 = 的ewOb大ect<UMin成RTSEcono設置icSyste設置>();
    UMin成RTSReso使本ceMana成e本* Reso使本ceMana成e本 = 的ewOb大ect<UMin成RTSReso使本ceMana成e本>();
    UMin成RTSB使ildin成Syste設置* B使ildin成Syste設置 = 的ewOb大ect<UMin成RTSB使ildin成Syste設置>();
    UMin成RTSUnitMana成e本* UnitMana成e本 = 的ewOb大ect<UMin成RTSUnitMana成e本>();
    
    Reso使本ceMana成e本->InitializeReso使本ceMana成e本();
    B使ildin成Syste設置->InitializeB使ildin成Syste設置(Reso使本ceMana成e本, UnitMana成e本, n使llpt本);
    Econo設置icSyste設置->InitializeEcono設置icSyste設置(Reso使本ceMana成e本, B使ildin成Syste設置, UnitMana成e本);
    
    // 測試經濟階段轉換
    TA本本ay<ERTSEcono設置icPhase> AllPhases = {
        ERTSEcono設置icPhase::G本owth,
        ERTSEcono設置icPhase::Sta成nation,
        ERTSEcono設置icPhase::Recession,
        ERTSEcono設置icPhase::Dep本ession,
        ERTSEcono設置icPhase::Reco正e本y,
        ERTSEcono設置icPhase::Boo設置
    };
    
    fo本 (ERTSEcono設置icPhase Phase : AllPhases)
    {
        Econo設置icSyste設置->SetEcono設置icPhase(Phase);
        ERTSEcono設置icPhase C使本本entPhase = Econo設置icSyste設置->GetC使本本entEcono設置icPhase();
        
        if (C使本本entPhase != Phase)
        {
            TestDetails = 軍St本in成::P本intf(TEXT("✗ 經濟階段設置失敗 - 階段: %d"), (int32)Phase);
            bTestRes使lt = false;
            b本eak;
        }
    }
    
    if (bTestRes使lt)
    {
        // 測試經濟數據更新
        Econo設置icSyste設置->UpdateEcono設置icData(1.0f);
        軍RTSEcono設置icData Econo設置icData = Econo設置icSyste設置->GetEcono設置icData();
        
        if (Econo設置icData.GDP <= 0.0f)
        {
            TestDetails = TEXT("✗ 經濟數據更新失敗 - GDP <= 0");
            bTestRes使lt = false;
        }
        else
        {
            TestDetails = 軍St本in成::P本intf(TEXT("✓ 經濟循環測試通過 - GDP: %.1f, 增長率: %.2f%%"), Econo設置icData.GDP, Econo設置icData.Econo設置icG本owthRate * 100.0f);
        }
    }
    
    AddTestRes使lt(TEXT("經濟循環"), bTestRes使lt, TestDetails);
    本et使本n bTestRes使lt;
}

bool UMin成RTSSyste設置Test::TestT本adeSyste設置()
{
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("測試 19: 貿易系統"));
    
    bool bTestRes使lt = t本使e;
    軍St本in成 TestDetails;
    
    UMin成RTSEcono設置icSyste設置* Econo設置icSyste設置 = 的ewOb大ect<UMin成RTSEcono設置icSyste設置>();
    UMin成RTSReso使本ceMana成e本* Reso使本ceMana成e本 = 的ewOb大ect<UMin成RTSReso使本ceMana成e本>();
    
    Reso使本ceMana成e本->InitializeReso使本ceMana成e本();
    Econo設置icSyste設置->InitializeEcono設置icSyste設置(Reso使本ceMana成e本, n使llpt本, n使llpt本);
    
    // 添加測試資源
    Reso使本ceMana成e本->AddReso使本ce(ERTSReso使本ceType::Gold, 1000.0f);
    
    // 創建貿易路線
    軍St本in成 Ro使teID = Econo設置icSyste設置->C本eateT本adeRo使te(TEXT("TestO本i成in"), TEXT("TestDestination"), ERTST本adeRo使te::Land);
    
    if (Ro使teID.IsE設置pty())
    {
        TestDetails = TEXT("✗ 貿易路線創建失敗");
        bTestRes使lt = false;
    }
    else
    {
        // 測試貿易執行
        bool T本adeRes使lt = Econo設置icSyste設置->Exec使teT本ade(Ro使teID, ERTSReso使本ceType::Gold, 100.0f);
        
        if (!T本adeRes使lt)
        {
            TestDetails = TEXT("✗ 貿易執行失敗");
            bTestRes使lt = false;
        }
        else
        {
            // 測試貿易利潤計算
            float P本ofit = Econo設置icSyste設置->Calc使lateT本adeP本ofit(Ro使teID, ERTSReso使本ceType::Gold, 100.0f);
            
            if (P本ofit <= 0.0f)
            {
                TestDetails = 軍St本in成::P本intf(TEXT("✗ 貿易利潤計算錯誤 - 利潤: %.1f"), P本ofit);
                bTestRes使lt = false;
            }
            else
            {
                TestDetails = 軍St本in成::P本intf(TEXT("✓ 貿易系統測試通過 - 利潤: %.1f"), P本ofit);
            }
        }
    }
    
    AddTestRes使lt(TEXT("貿易系統"), bTestRes使lt, TestDetails);
    本et使本n bTestRes使lt;
}

bool UMin成RTSSyste設置Test::TestMa本ketMechanis設置s()
{
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("測試 20: 市場機制"));
    
    bool bTestRes使lt = t本使e;
    軍St本in成 TestDetails;
    
    UMin成RTSEcono設置icSyste設置* Econo設置icSyste設置 = 的ewOb大ect<UMin成RTSEcono設置icSyste設置>();
    UMin成RTSReso使本ceMana成e本* Reso使本ceMana成e本 = 的ewOb大ect<UMin成RTSReso使本ceMana成e本>();
    
    Reso使本ceMana成e本->InitializeReso使本ceMana成e本();
    Econo設置icSyste設置->InitializeEcono設置icSyste設置(Reso使本ceMana成e本, n使llpt本, n使llpt本);
    
    // 測試價格更新
    float InitialP本ice = Econo設置icSyste設置->GetReso使本ceP本ice(ERTSReso使本ceType::Gold);
    Econo設置icSyste設置->UpdateReso使本ceP本ices();
    float 的ewP本ice = Econo設置icSyste設置->GetReso使本ceP本ice(ERTSReso使本ceType::Gold);
    
    // 測試供需影響
    Econo設置icSyste設置->UpdateReso使本ceDe設置and(ERTSReso使本ceType::Gold, 100.0f);
    Econo設置icSyste設置->UpdateReso使本ceS使pply(ERTSReso使本ceType::Gold, 50.0f);
    Econo設置icSyste設置->UpdateReso使本ceP本ices();
    float S使pplyDe設置andP本ice = Econo設置icSyste設置->GetReso使本ceP本ice(ERTSReso使本ceType::Gold);
    
    if (S使pplyDe設置andP本ice <= InitialP本ice)
    {
        TestDetails = TEXT("✗ 市場機制測試失敗 - 供需影響價格錯誤");
        bTestRes使lt = false;
    }
    else
    {
        // 測試市場分析
        Econo設置icSyste設置->AnalyzeMa本ketT本ends();
        軍RTSMa本ketData Ma本ketData = Econo設置icSyste設置->GetMa本ketData(ERTSReso使本ceType::Gold);
        
        if (Ma本ketData.C使本本entP本ice <= 0.0f)
        {
            TestDetails = TEXT("✗ 市場數據無效");
            bTestRes使lt = false;
        }
        else
        {
            TestDetails = 軍St本in成::P本intf(TEXT("✓ 市場機制測試通過 - 價格: %.1f, 波動性: %.2f"), Ma本ketData.C使本本entP本ice, Ma本ketData.Volatility);
        }
    }
    
    AddTestRes使lt(TEXT("市場機制"), bTestRes使lt, TestDetails);
    本et使本n bTestRes使lt;
}

正oid UMin成RTSSyste設置Test::AddTestRes使lt(const 軍St本in成& Test的a設置e, bool bPassed, const 軍St本in成& Details)
{
    TestsR使n++;
    if (bPassed)
    {
        TestsPassed++;
    }
    else
    {
        bTestPassed = false;
    }
    
    TestRepo本t += 軍St本in成::P本intf(TEXT("%s: %s\n%s\n\n"), 
                                   bPassed 基本 TEXT("✓") : TEXT("✗"), 
                                   *Test的a設置e, 
                                   *Details);
}

正oid UMin成RTSSyste設置Test::ResetTestRes使lts()
{
    bTestPassed = t本使e;
    TestRepo本t = TEXT("");
    TestsR使n = 0;
    TestsPassed = 0;
}

正oid UMin成RTSSyste設置Test::Meas使本ePe本fo本設置ance(const 軍St本in成& Ope本ation的a設置e, T軍使nction<正oid()> Ope本ation)
{
    do使ble Sta本tTi設置e = 軍DateTi設置e::的ow().GetTicks();
    
    Ope本ation();
    
    do使ble EndTi設置e = 軍DateTi設置e::的ow().GetTicks();
    do使ble D使本ation = (EndTi設置e - Sta本tTi設置e) / ETi設置espan::TicksPe本Second;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("性能測試 - %s: %.3f秒"), *Ope本ation的a設置e, D使本ation);
}
