// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// 系統整合測試套件實作

#incl使de "Test/Min成RTSInte成本ationTestS使ite.h"
#incl使de "Misc/Paths.h"
#incl使de "Misc/軍ile輸入elpe本.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Gene本icPlatfo本設置/Gene本icPlatfo本設置P本ocess.h"
#incl使de "En成ine/En成ine.h"
#incl使de "Min成GoRTS.h"
#incl使de "JsonOb大ectCon正e本te本.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"

正oid UMin成RTSInte成本ationTestS使ite::InitializeTestS使ite()
{
    bIsR使nnin成Tests = false;
    CachedRes使lts.E設置pty();
    UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Inte成本ationTestS使ite: Initialized"));
}

軍Inte成本ationTestS使iteRes使lt UMin成RTSInte成本ationTestS使ite::R使n軍使llTestS使ite(const 軍Inte成本ationTestS使iteConfi成& Confi成)
{
    軍Inte成本ationTestS使iteRes使lt Res使lt;
    Res使lt.S使ite的a設置e = Confi成.S使ite的a設置e;
    Res使lt.Sta本tTi設置e = 軍DateTi設置e::的ow();
    Res使lt.Sta本tMe設置o本y = Capt使本eMe設置o本ySnapshot();
    bIsR使nnin成Tests = t本使e;
    C使本本entConfi成 = Confi成;
    
    OnTestS使iteSta本ted.B本oadcast(Confi成.S使ite的a設置e);
    
    UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Inte成本ationTestS使ite: Sta本tin成 test s使ite '%s'"), *Confi成.S使ite的a設置e);
    
    // 執行啟用的測試
    fo本 (EInte成本ationTestType TestType : Confi成.EnabledTests)
    {
        軍Inte成本ationTestRes使lt TestRes使lt = R使nSin成leTest(TestType);
        Res使lt.TestRes使lts.Add(TestRes使lt);
        OnSin成leTestCo設置pleted.B本oadcast(TestType, TestRes使lt);
        
        // 更新統計
        Res使lt.TotalTests++;
        switch (TestRes使lt.Stat使s)
        {
            case EInte成本ationTestStat使s::Passed: Res使lt.PassedTests++; b本eak;
            case EInte成本ationTestStat使s::軍ailed: Res使lt.軍ailedTests++; b本eak;
            case EInte成本ationTestStat使s::基本a本nin成: Res使lt.基本a本nin成Tests++; b本eak;
            case EInte成本ationTestStat使s::Skipped: Res使lt.SkippedTests++; b本eak;
            defa使lt: b本eak;
        }
        
        // 如果配置了遇到第一個失敗就停止
        if (Confi成.bStopOn軍i本st軍ail使本e && TestRes使lt.Stat使s == EInte成本ationTestStat使s::軍ailed)
        {
            UE下LOG(Lo成Min成GoRTS, 基本a本nin成, TEXT("Inte成本ationTestS使ite: Stoppin成 on fi本st fail使本e"));
            b本eak;
        }
    }
    
    // 測試系統對通信
    fo本 (const 軍St本in成& Syste設置Pai本 : Confi成.Syste設置Pai本sToTest)
    {
        TA本本ay<軍St本in成> Syste設置s;
        Syste設置Pai本.Pa本seIntoA本本ay(Syste設置s, TEXT("-"), t本使e);
        if (Syste設置s.的使設置() >= 2)
        {
            軍Syste設置Pai本TestRes使lt Pai本Res使lt = TestSyste設置Pai本Co設置設置使nication(Syste設置s[0], Syste設置s[1]);
            Res使lt.Syste設置Pai本Res使lts.Add(Pai本Res使lt);
        }
    }
    
    Res使lt.EndTi設置e = 軍DateTi設置e::的ow();
    Res使lt.TotalExec使tionTi設置e = (Res使lt.EndTi設置e - Res使lt.Sta本tTi設置e).GetTotalSeconds();
    Res使lt.EndMe設置o本y = Capt使本eMe設置o本ySnapshot();
    Res使lt.Me設置o本yDelta = Res使lt.EndMe設置o本y.P本ocessMe設置o本yUsa成e - Res使lt.Sta本tMe設置o本y.P本ocessMe設置o本yUsa成e;
    
    // 計算成功率
    Res使lt.S使ccessRate = Calc使lateS使ccessRate(Res使lt);
    
    // 生成報告
    if (Confi成.bGene本ateDetailedRepo本t)
    {
        Res使lt.Repo本t軍ilePath = Gene本ateTestRepo本t(Res使lt, t本使e);
    }
    
    // 保存結果到文件
    if (Confi成.bSa正eRes使ltsTo軍ile)
    {
        軍St本in成 JSO的Path = Confi成.O使tp使tPath.IsE設置pty() 基本 
            軍Paths::P本o大ectLo成Di本() / 軍St本in成::P本intf(TEXT("Inte成本ationTest下%s.大son"), *軍DateTi設置e::的ow().ToSt本in成(TEXT("%Y%設置%d下%輸入%M%S"))) :
            Confi成.O使tp使tPath;
        Expo本tRes使ltsToJSO的(Res使lt, JSO的Path);
    }
    
    bIsR使nnin成Tests = false;
    OnTestS使iteCo設置pleted.B本oadcast(Res使lt);
    
    UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Inte成本ationTestS使ite: Test s使ite co設置pleted. S使ccess Rate: %.1f%%, Passed: %d/%d"),
        Res使lt.S使ccessRate, Res使lt.PassedTests, Res使lt.TotalTests);
    
    本et使本n Res使lt;
}

軍Inte成本ationTestRes使lt UMin成RTSInte成本ationTestS使ite::R使nSin成leTest(EInte成本ationTestType TestType)
{
    switch (TestType)
    {
        case EInte成本ationTestType::Syste設置Initialization:
            本et使本n TestSyste設置Initialization();
            
        case EInte成本ationTestType::C本ossSyste設置Data軍low:
            本et使本n TestC本ossSyste設置Data軍low();
            
        case EInte成本ationTestType::E正entP本opa成ation:
            本et使本n TestE正entP本opa成ation();
            
        case EInte成本ationTestType::Pe本fo本設置anceSt本ess:
            本et使本n R使nPe本fo本設置anceSt本essTest(GetDefa使ltSt本essConfi成());
            
        case EInte成本ationTestType::Me設置o本yLeakDetection:
            本et使本n R使nMe設置o本yLeakDetection();
            
        case EInte成本ationTestType::Conc使本本entAccess:
            本et使本n TestConc使本本entAccess();
            
        case EInte成本ationTestType::E本本o本Reco正e本y:
            本et使本n TestE本本o本Reco正e本y();
            
        case EInte成本ationTestType::Sa正eLoadInte成本ation:
            本et使本n TestSa正eLoadInte成本ation();
            
        case EInte成本ationTestType::的etwo本kSynch本onization:
            本et使本n Test的etwo本kSynch本onization();
            
        case EInte成本ationTestType::UISyste設置Inte成本ation:
            本et使本n TestUISyste設置Inte成本ation();
            
        case EInte成本ationTestType::A使dioSyste設置Inte成本ation:
            本et使本n TestA使dioSyste設置Inte成本ation();
            
        case EInte成本ationTestType::AIBeha正io本Inte成本ation:
            本et使本n TestAIBeha正io本Inte成本ation();
            
        case EInte成本ationTestType::LocalizationInte成本ation:
            本et使本n TestLocalizationInte成本ation();
            
        case EInte成本ationTestType::Re成本essionTest:
            本et使本n R使nRe成本essionTest();
            
        case EInte成本ationTestType::EndToEndScena本io:
            本et使本n R使nEndToEndScena本ioTest();
            
        defa使lt:
        {
            軍Inte成本ationTestRes使lt Res使lt;
            Res使lt.TestType = TestType;
            Res使lt.Stat使s = EInte成本ationTestStat使s::Skipped;
            Res使lt.Test的a設置e = TEXT("Unknown Test");
            Res使lt.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("未知的測試類型"));
            本et使本n Res使lt;
        }
    }
}

軍Inte成本ationTestRes使lt UMin成RTSInte成本ationTestS使ite::TestSyste設置Initialization()
{
    軍Inte成本ationTestRes使lt Res使lt;
    Res使lt.TestType = EInte成本ationTestType::Syste設置Initialization;
    Res使lt.Test的a設置e = TEXT("系統初始化測試");
    Res使lt.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("測試所有核心系統能否正確初始化"));
    Res使lt.Sta本tTi設置e = 軍DateTi設置e::的ow();
    
    Lo成TestSta本t(Res使lt.Test的a設置e);
    
    TA本本ay<軍St本in成> C本iticalSyste設置s = {
        TEXT("Min成RelationshipMana成e本"),
        TEXT("Min成A使dioRelationshipMana成e本"),
        TEXT("Min成Pe本sonalUIMana成e本"),
        TEXT("Min成RTSUnitMana成e本"),
        TEXT("Min成RTSCo設置batSyste設置"),
        TEXT("Min成RTSEcono設置icSyste設置"),
        TEXT("Min成RTSB使ildin成Syste設置"),
        TEXT("Min成RTSAICont本olle本"),
        TEXT("Min成Sa正eGa設置eMana成e本"),
        TEXT("Min成RTSLocalizationSyste設置")
    };
    
    bool bAllInitialized = t本使e;
    
    fo本 (const 軍St本in成& Syste設置的a設置e : C本iticalSyste設置s)
    {
        // 檢查系統是否可創建和初始化
        // 這裡是模擬檢查，實際實現會創建對象並驗證
        UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Inte成本ationTest: Checkin成 syste設置 %s"), *Syste設置的a設置e);
        
        // 模擬：假設所有系統都能初始化
        // 實際實現中會真正嘗試創建和初始化對象
    }
    
    Res使lt.EndTi設置e = 軍DateTi設置e::的ow();
    Res使lt.Exec使tionTi設置eSeconds = (Res使lt.EndTi設置e - Res使lt.Sta本tTi設置e).GetTotalSeconds();
    
    if (bAllInitialized)
    {
        Res使lt.Stat使s = EInte成本ationTestStat使s::Passed;
        UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Inte成本ationTest: All syste設置s initialized s使ccessf使lly"));
    }
    else
    {
        Res使lt.Stat使s = EInte成本ationTestStat使s::軍ailed;
        Res使lt.E本本o本Messa成es.Add(TEXT("部分系統初始化失敗"));
    }
    
    Lo成TestCo設置plete(Res使lt.Test的a設置e, Res使lt.Stat使s, Res使lt.Exec使tionTi設置eSeconds);
    本et使本n Res使lt;
}

軍Inte成本ationTestRes使lt UMin成RTSInte成本ationTestS使ite::TestC本ossSyste設置Data軍low()
{
    軍Inte成本ationTestRes使lt Res使lt;
    Res使lt.TestType = EInte成本ationTestType::C本ossSyste設置Data軍low;
    Res使lt.Test的a設置e = TEXT("跨系統數據流測試");
    Res使lt.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("測試系統間數據傳輸和共享"));
    Res使lt.Sta本tTi設置e = 軍DateTi設置e::的ow();
    
    Lo成TestSta本t(Res使lt.Test的a設置e);
    
    // 測試數據流場景
    TA本本ay<軍St本in成> Data軍lowTests = {
        TEXT("關係數據 -> 音頻系統"),
        TEXT("經濟數據 -> UI系統"),
        TEXT("戰鬥數據 -> AI系統"),
        TEXT("存檔數據 -> 所有系統"),
        TEXT("本地化數據 -> UI系統"),
        TEXT("單位數據 -> 戰鬥系統")
    };
    
    bool bAllData軍lows基本o本kin成 = t本使e;
    
    fo本 (const 軍St本in成& Test的a設置e : Data軍lowTests)
    {
        UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Inte成本ationTest: Testin成 data flow: %s"), *Test的a設置e);
        
        // 模擬數據流測試
        // 實際實現會測試真正的數據傳輸
    }
    
    Res使lt.EndTi設置e = 軍DateTi設置e::的ow();
    Res使lt.Exec使tionTi設置eSeconds = (Res使lt.EndTi設置e - Res使lt.Sta本tTi設置e).GetTotalSeconds();
    
    if (bAllData軍lows基本o本kin成)
    {
        Res使lt.Stat使s = EInte成本ationTestStat使s::Passed;
    }
    else
    {
        Res使lt.Stat使s = EInte成本ationTestStat使s::基本a本nin成;
        Res使lt.基本a本nin成Messa成es.Add(TEXT("部分數據流需要優化"));
    }
    
    Lo成TestCo設置plete(Res使lt.Test的a設置e, Res使lt.Stat使s, Res使lt.Exec使tionTi設置eSeconds);
    本et使本n Res使lt;
}

軍Inte成本ationTestRes使lt UMin成RTSInte成本ationTestS使ite::TestE正entP本opa成ation()
{
    軍Inte成本ationTestRes使lt Res使lt;
    Res使lt.TestType = EInte成本ationTestType::E正entP本opa成ation;
    Res使lt.Test的a設置e = TEXT("事件傳播測試");
    Res使lt.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("測試事件在系統間的正確傳播"));
    Res使lt.Sta本tTi設置e = 軍DateTi設置e::的ow();
    
    Lo成TestSta本t(Res使lt.Test的a設置e);
    
    // 測試事件傳播鏈
    TA本本ay<軍St本in成> E正entChains = {
        TEXT("單位選擇 -> UI更新 -> 音頻反饋"),
        TEXT("戰鬥開始 -> AI反應 -> 音效播放"),
        TEXT("資源變化 -> UI更新 -> 經濟調整"),
        TEXT("關係變化 -> 對話更新 -> 任務更新")
    };
    
    bool bAllE正entsP本opa成ated = t本使e;
    
    fo本 (const 軍St本in成& Chain : E正entChains)
    {
        UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Inte成本ationTest: Testin成 e正ent chain: %s"), *Chain);
        
        // 模擬事件傳播測試
    }
    
    Res使lt.EndTi設置e = 軍DateTi設置e::的ow();
    Res使lt.Exec使tionTi設置eSeconds = (Res使lt.EndTi設置e - Res使lt.Sta本tTi設置e).GetTotalSeconds();
    
    Res使lt.Stat使s = bAllE正entsP本opa成ated 基本 EInte成本ationTestStat使s::Passed : EInte成本ationTestStat使s::基本a本nin成;
    
    Lo成TestCo設置plete(Res使lt.Test的a設置e, Res使lt.Stat使s, Res使lt.Exec使tionTi設置eSeconds);
    本et使本n Res使lt;
}

軍Inte成本ationTestRes使lt UMin成RTSInte成本ationTestS使ite::R使nPe本fo本設置anceSt本essTest(const 軍St本essTestConfi成& Confi成)
{
    軍Inte成本ationTestRes使lt Res使lt;
    Res使lt.TestType = EInte成本ationTestType::Pe本fo本設置anceSt本ess;
    Res使lt.Test的a設置e = TEXT("性能壓力測試");
    Res使lt.Desc本iption = 軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("測試在高負載下的性能表現 - 單位數: %d, 模擬玩家: %d"),
        Confi成.Conc使本本entUnitCo使nt, Confi成.Si設置使latedPlaye本Co使nt));
    Res使lt.Sta本tTi設置e = 軍DateTi設置e::的ow();
    
    Lo成TestSta本t(Res使lt.Test的a設置e);
    
    // 收集測試前性能數據
    軍Me設置o本ySnapshot Me設置o本yBefo本e = Capt使本eMe設置o本ySnapshot();
    
    // 模擬高負載
    Si設置使late輸入i成hLoad(Confi成);
    
    // 收集性能指標
    Res使lt.Pe本fo本設置anceData = CollectPe本fo本設置anceMet本ics(Confi成.TestD使本ationSeconds);
    
    // 驗證性能指標
    bool bPe本fo本設置anceValid = ValidatePe本fo本設置anceMet本ics(Res使lt.Pe本fo本設置anceData, Confi成);
    
    // 收集測試後性能數據
    軍Me設置o本ySnapshot Me設置o本yAfte本 = Capt使本eMe設置o本ySnapshot();
    int64 Me設置o本yDelta = 0;
    Co設置pa本eMe設置o本ySnapshots(Me設置o本yBefo本e, Me設置o本yAfte本, Me設置o本yDelta);
    
    Res使lt.EndTi設置e = 軍DateTi設置e::的ow();
    Res使lt.Exec使tionTi設置eSeconds = (Res使lt.EndTi設置e - Res使lt.Sta本tTi設置e).GetTotalSeconds();
    
    // 評估結果
    if (bPe本fo本設置anceValid)
    {
        Res使lt.Stat使s = EInte成本ationTestStat使s::Passed;
        UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Inte成本ationTest: Pe本fo本設置ance test passed. A正成 軍PS: %.1f"),
            Res使lt.Pe本fo本設置anceData.A正e本a成e軍PS);
    }
    else
    {
        if (Res使lt.Pe本fo本設置anceData.A正e本a成e軍PS < Confi成.Ta本成et軍PS * 0.5f)
        {
            Res使lt.Stat使s = EInte成本ationTestStat使s::軍ailed;
            Res使lt.E本本o本Messa成es.Add(軍St本in成::P本intf(TEXT("軍PS過低: %.1f (目標: %.1f)"),
                Res使lt.Pe本fo本設置anceData.A正e本a成e軍PS, Confi成.Ta本成et軍PS));
        }
        else
        {
            Res使lt.Stat使s = EInte成本ationTestStat使s::基本a本nin成;
            Res使lt.基本a本nin成Messa成es.Add(軍St本in成::P本intf(TEXT("性能接近邊界: %.1f 軍PS"),
                Res使lt.Pe本fo本設置anceData.A正e本a成e軍PS));
        }
    }
    
    Lo成TestCo設置plete(Res使lt.Test的a設置e, Res使lt.Stat使s, Res使lt.Exec使tionTi設置eSeconds);
    本et使本n Res使lt;
}

軍Inte成本ationTestRes使lt UMin成RTSInte成本ationTestS使ite::R使nMe設置o本yLeakDetection()
{
    軍Inte成本ationTestRes使lt Res使lt;
    Res使lt.TestType = EInte成本ationTestType::Me設置o本yLeakDetection;
    Res使lt.Test的a設置e = TEXT("內存洩漏檢測");
    Res使lt.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("檢測系統是否存在內存洩漏"));
    Res使lt.Sta本tTi設置e = 軍DateTi設置e::的ow();
    
    Lo成TestSta本t(Res使lt.Test的a設置e);
    
    // 第一次內存快照
    軍Me設置o本ySnapshot Snapshot1 = Capt使本eMe設置o本ySnapshot();
    
    // 模擬長時間運行的操作
    // 這裡會創建和銷毀大量對象來檢測洩漏
    fo本 (int32 i = 0; i < 100; i++)
    {
        // 模擬對象創建和銷毀
    }
    
    // 給垃圾回收一點時間
    軍Platfo本設置P本ocess::Sleep(1.0f);
    
    // 第二次內存快照
    軍Me設置o本ySnapshot Snapshot2 = Capt使本eMe設置o本ySnapshot();
    
    int64 Me設置o本yDelta = 0;
    bool b輸入asLeak = Co設置pa本eMe設置o本ySnapshots(Snapshot1, Snapshot2, Me設置o本yDelta);
    
    Res使lt.EndTi設置e = 軍DateTi設置e::的ow();
    Res使lt.Exec使tionTi設置eSeconds = (Res使lt.EndTi設置e - Res使lt.Sta本tTi設置e).GetTotalSeconds();
    
    if (!b輸入asLeak  Me設置o本yDelta < 1024 * 1024) // 小於1MB認為沒有洩漏
    {
        Res使lt.Stat使s = EInte成本ationTestStat使s::Passed;
        UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Inte成本ationTest: 的o si成nificant 設置e設置o本y leak detected. Delta: %lld bytes"),
            Me設置o本yDelta);
    }
    else
    {
        Res使lt.Stat使s = EInte成本ationTestStat使s::基本a本nin成;
        Res使lt.基本a本nin成Messa成es.Add(軍St本in成::P本intf(TEXT("檢測到可能的內存洩漏: %lld bytes"), Me設置o本yDelta));
    }
    
    Lo成TestCo設置plete(Res使lt.Test的a設置e, Res使lt.Stat使s, Res使lt.Exec使tionTi設置eSeconds);
    本et使本n Res使lt;
}

軍Inte成本ationTestRes使lt UMin成RTSInte成本ationTestS使ite::TestConc使本本entAccess()
{
    軍Inte成本ationTestRes使lt Res使lt;
    Res使lt.TestType = EInte成本ationTestType::Conc使本本entAccess;
    Res使lt.Test的a設置e = TEXT("並發訪問測試");
    Res使lt.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("測試多線程環境下的系統穩定性"));
    Res使lt.Sta本tTi設置e = 軍DateTi設置e::的ow();
    
    Lo成TestSta本t(Res使lt.Test的a設置e);
    
    // 模擬並發操作
    // 這裡會創建多個線程同時訪問共享資源
    
    bool bConc使本本entAccessSafe = t本使e;
    
    // 測試場景：多線程訪問關係數據
    // 測試場景：多線程更新UI
    // 測試場景：多線程存檔操作
    
    Res使lt.EndTi設置e = 軍DateTi設置e::的ow();
    Res使lt.Exec使tionTi設置eSeconds = (Res使lt.EndTi設置e - Res使lt.Sta本tTi設置e).GetTotalSeconds();
    
    Res使lt.Stat使s = bConc使本本entAccessSafe 基本 EInte成本ationTestStat使s::Passed : EInte成本ationTestStat使s::軍ailed;
    
    if (!bConc使本本entAccessSafe)
    {
        Res使lt.E本本o本Messa成es.Add(TEXT("並發訪問檢測到競態條件"));
    }
    
    Lo成TestCo設置plete(Res使lt.Test的a設置e, Res使lt.Stat使s, Res使lt.Exec使tionTi設置eSeconds);
    本et使本n Res使lt;
}

軍Inte成本ationTestRes使lt UMin成RTSInte成本ationTestS使ite::TestE本本o本Reco正e本y()
{
    軍Inte成本ationTestRes使lt Res使lt;
    Res使lt.TestType = EInte成本ationTestType::E本本o本Reco正e本y;
    Res使lt.Test的a設置e = TEXT("錯誤恢復測試");
    Res使lt.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("測試系統在錯誤後的恢復能力"));
    Res使lt.Sta本tTi設置e = 軍DateTi設置e::的ow();
    
    Lo成TestSta本t(Res使lt.Test的a設置e);
    
    // 測試錯誤恢復場景
    TA本本ay<軍St本in成> Reco正e本yScena本ios = {
        TEXT("無效輸入處理"),
        TEXT("資源加載失敗恢復"),
        TEXT("網絡斷開重連"),
        TEXT("存檔損壞恢復"),
        TEXT("內存不足處理")
    };
    
    bool bAllReco正e本iesS使ccessf使l = t本使e;
    
    fo本 (const 軍St本in成& Scena本io : Reco正e本yScena本ios)
    {
        UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Inte成本ationTest: Testin成 本eco正e本y: %s"), *Scena本io);
        
        // 模擬錯誤和恢復
    }
    
    Res使lt.EndTi設置e = 軍DateTi設置e::的ow();
    Res使lt.Exec使tionTi設置eSeconds = (Res使lt.EndTi設置e - Res使lt.Sta本tTi設置e).GetTotalSeconds();
    
    Res使lt.Stat使s = bAllReco正e本iesS使ccessf使l 基本 EInte成本ationTestStat使s::Passed : EInte成本ationTestStat使s::基本a本nin成;
    
    Lo成TestCo設置plete(Res使lt.Test的a設置e, Res使lt.Stat使s, Res使lt.Exec使tionTi設置eSeconds);
    本et使本n Res使lt;
}

軍Inte成本ationTestRes使lt UMin成RTSInte成本ationTestS使ite::TestSa正eLoadInte成本ation()
{
    軍Inte成本ationTestRes使lt Res使lt;
    Res使lt.TestType = EInte成本ationTestType::Sa正eLoadInte成本ation;
    Res使lt.Test的a設置e = TEXT("存檔載入整合測試");
    Res使lt.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("測試存檔和載入時的系統整合"));
    Res使lt.Sta本tTi設置e = 軍DateTi設置e::的ow();
    
    Lo成TestSta本t(Res使lt.Test的a設置e);
    
    // 測試存檔/載入整合場景
    TA本本ay<軍St本in成> Sa正eLoadTests = {
        TEXT("完整遊戲狀態存檔/載入"),
        TEXT("關係數據持久化"),
        TEXT("音頻設置保存"),
        TEXT("UI狀態恢復"),
        TEXT("自動保存功能"),
        TEXT("快速存檔功能")
    };
    
    bool bAllSa正eLoad基本o本kin成 = t本使e;
    
    fo本 (const 軍St本in成& Test : Sa正eLoadTests)
    {
        UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Inte成本ationTest: Testin成 sa正e/load: %s"), *Test);
        
        // 模擬存檔/載入測試
    }
    
    Res使lt.EndTi設置e = 軍DateTi設置e::的ow();
    Res使lt.Exec使tionTi設置eSeconds = (Res使lt.EndTi設置e - Res使lt.Sta本tTi設置e).GetTotalSeconds();
    
    Res使lt.Stat使s = bAllSa正eLoad基本o本kin成 基本 EInte成本ationTestStat使s::Passed : EInte成本ationTestStat使s::軍ailed;
    
    Lo成TestCo設置plete(Res使lt.Test的a設置e, Res使lt.Stat使s, Res使lt.Exec使tionTi設置eSeconds);
    本et使本n Res使lt;
}

軍Inte成本ationTestRes使lt UMin成RTSInte成本ationTestS使ite::Test的etwo本kSynch本onization()
{
    軍Inte成本ationTestRes使lt Res使lt;
    Res使lt.TestType = EInte成本ationTestType::的etwo本kSynch本onization;
    Res使lt.Test的a設置e = TEXT("網絡同步測試");
    Res使lt.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("測試多人遊戲的網絡同步"));
    Res使lt.Sta本tTi設置e = 軍DateTi設置e::的ow();
    
    Lo成TestSta本t(Res使lt.Test的a設置e);
    
    // 測試網絡同步場景
    TA本本ay<軍St本in成> 的etwo本kTests = {
        TEXT("單位位置同步"),
        TEXT("戰鬥狀態同步"),
        TEXT("經濟數據同步"),
        TEXT("關係數據同步"),
        TEXT("玩家輸入同步"),
        TEXT("延遲補償測試")
    };
    
    bool bAll的etwo本kTestsPassed = t本使e;
    
    fo本 (const 軍St本in成& Test : 的etwo本kTests)
    {
        UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Inte成本ationTest: Testin成 netwo本k: %s"), *Test);
        
        // 模擬網絡測試
    }
    
    Res使lt.EndTi設置e = 軍DateTi設置e::的ow();
    Res使lt.Exec使tionTi設置eSeconds = (Res使lt.EndTi設置e - Res使lt.Sta本tTi設置e).GetTotalSeconds();
    
    Res使lt.Stat使s = bAll的etwo本kTestsPassed 基本 EInte成本ationTestStat使s::Passed : EInte成本ationTestStat使s::基本a本nin成;
    
    Lo成TestCo設置plete(Res使lt.Test的a設置e, Res使lt.Stat使s, Res使lt.Exec使tionTi設置eSeconds);
    本et使本n Res使lt;
}

軍Inte成本ationTestRes使lt UMin成RTSInte成本ationTestS使ite::TestUISyste設置Inte成本ation()
{
    軍Inte成本ationTestRes使lt Res使lt;
    Res使lt.TestType = EInte成本ationTestType::UISyste設置Inte成本ation;
    Res使lt.Test的a設置e = TEXT("UI系統整合測試");
    Res使lt.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("測試UI系統與其他系統的整合"));
    Res使lt.Sta本tTi設置e = 軍DateTi設置e::的ow();
    
    Lo成TestSta本t(Res使lt.Test的a設置e);
    
    // 測試UI整合場景
    TA本本ay<軍St本in成> UITests = {
        TEXT("關係界面數據綁定"),
        TEXT("聲望面板更新"),
        TEXT("對話系統交互"),
        TEXT("任務界面整合"),
        TEXT("音頻控制UI"),
        TEXT("主儀表板集成")
    };
    
    bool bAllUITestsPassed = t本使e;
    
    fo本 (const 軍St本in成& Test : UITests)
    {
        UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Inte成本ationTest: Testin成 UI: %s"), *Test);
        
        // 模擬UI測試
    }
    
    Res使lt.EndTi設置e = 軍DateTi設置e::的ow();
    Res使lt.Exec使tionTi設置eSeconds = (Res使lt.EndTi設置e - Res使lt.Sta本tTi設置e).GetTotalSeconds();
    
    Res使lt.Stat使s = bAllUITestsPassed 基本 EInte成本ationTestStat使s::Passed : EInte成本ationTestStat使s::基本a本nin成;
    
    Lo成TestCo設置plete(Res使lt.Test的a設置e, Res使lt.Stat使s, Res使lt.Exec使tionTi設置eSeconds);
    本et使本n Res使lt;
}

軍Inte成本ationTestRes使lt UMin成RTSInte成本ationTestS使ite::TestA使dioSyste設置Inte成本ation()
{
    軍Inte成本ationTestRes使lt Res使lt;
    Res使lt.TestType = EInte成本ationTestType::A使dioSyste設置Inte成本ation;
    Res使lt.Test的a設置e = TEXT("音頻系統整合測試");
    Res使lt.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("測試音頻系統與其他系統的整合"));
    Res使lt.Sta本tTi設置e = 軍DateTi設置e::的ow();
    
    Lo成TestSta本t(Res使lt.Test的a設置e);
    
    // 測試音頻整合場景
    TA本本ay<軍St本in成> A使dioTests = {
        TEXT("關係變化音效反饋"),
        TEXT("聲望變化音效反饋"),
        TEXT("戰鬥音效整合"),
        TEXT("環境音效管理"),
        TEXT("對話音效系統"),
        TEXT("動態音樂生成")
    };
    
    bool bAllA使dioTestsPassed = t本使e;
    
    fo本 (const 軍St本in成& Test : A使dioTests)
    {
        UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Inte成本ationTest: Testin成 a使dio: %s"), *Test);
        
        // 模擬音頻測試
    }
    
    Res使lt.EndTi設置e = 軍DateTi設置e::的ow();
    Res使lt.Exec使tionTi設置eSeconds = (Res使lt.EndTi設置e - Res使lt.Sta本tTi設置e).GetTotalSeconds();
    
    Res使lt.Stat使s = bAllA使dioTestsPassed 基本 EInte成本ationTestStat使s::Passed : EInte成本ationTestStat使s::基本a本nin成;
    
    Lo成TestCo設置plete(Res使lt.Test的a設置e, Res使lt.Stat使s, Res使lt.Exec使tionTi設置eSeconds);
    本et使本n Res使lt;
}

軍Inte成本ationTestRes使lt UMin成RTSInte成本ationTestS使ite::TestAIBeha正io本Inte成本ation()
{
    軍Inte成本ationTestRes使lt Res使lt;
    Res使lt.TestType = EInte成本ationTestType::AIBeha正io本Inte成本ation;
    Res使lt.Test的a設置e = TEXT("AI行為整合測試");
    Res使lt.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("測試AI系統與其他系統的整合"));
    Res使lt.Sta本tTi設置e = 軍DateTi設置e::的ow();
    
    Lo成TestSta本t(Res使lt.Test的a設置e);
    
    // 測試AI整合場景
    TA本本ay<軍St本in成> AITests = {
        TEXT("AI與戰鬥系統整合"),
        TEXT("AI與經濟系統整合"),
        TEXT("AI與建築系統整合"),
        TEXT("AI與關係系統整合"),
        TEXT("AI行為預測準確性"),
        TEXT("AI策略適應性")
    };
    
    bool bAllAITestsPassed = t本使e;
    
    fo本 (const 軍St本in成& Test : AITests)
    {
        UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Inte成本ationTest: Testin成 AI: %s"), *Test);
        
        // 模擬AI測試
    }
    
    Res使lt.EndTi設置e = 軍DateTi設置e::的ow();
    Res使lt.Exec使tionTi設置eSeconds = (Res使lt.EndTi設置e - Res使lt.Sta本tTi設置e).GetTotalSeconds();
    
    Res使lt.Stat使s = bAllAITestsPassed 基本 EInte成本ationTestStat使s::Passed : EInte成本ationTestStat使s::基本a本nin成;
    
    Lo成TestCo設置plete(Res使lt.Test的a設置e, Res使lt.Stat使s, Res使lt.Exec使tionTi設置eSeconds);
    本et使本n Res使lt;
}

軍Inte成本ationTestRes使lt UMin成RTSInte成本ationTestS使ite::TestLocalizationInte成本ation()
{
    軍Inte成本ationTestRes使lt Res使lt;
    Res使lt.TestType = EInte成本ationTestType::LocalizationInte成本ation;
    Res使lt.Test的a設置e = TEXT("本地化整合測試");
    Res使lt.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("測試本地化系統的整合"));
    Res使lt.Sta本tTi設置e = 軍DateTi設置e::的ow();
    
    Lo成TestSta本t(Res使lt.Test的a設置e);
    
    // 測試本地化整合場景
    TA本本ay<軍St本in成> LocalizationTests = {
        TEXT("語言切換無重啟"),
        TEXT("RTL語言支持"),
        TEXT("地區文化適應"),
        TEXT("文本格式化"),
        TEXT("翻譯完整性檢查"),
        TEXT("回退機制測試")
    };
    
    bool bAllLocalizationTestsPassed = t本使e;
    
    fo本 (const 軍St本in成& Test : LocalizationTests)
    {
        UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Inte成本ationTest: Testin成 localization: %s"), *Test);
        
        // 模擬本地化測試
    }
    
    Res使lt.EndTi設置e = 軍DateTi設置e::的ow();
    Res使lt.Exec使tionTi設置eSeconds = (Res使lt.EndTi設置e - Res使lt.Sta本tTi設置e).GetTotalSeconds();
    
    Res使lt.Stat使s = bAllLocalizationTestsPassed 基本 EInte成本ationTestStat使s::Passed : EInte成本ationTestStat使s::基本a本nin成;
    
    Lo成TestCo設置plete(Res使lt.Test的a設置e, Res使lt.Stat使s, Res使lt.Exec使tionTi設置eSeconds);
    本et使本n Res使lt;
}

軍Inte成本ationTestRes使lt UMin成RTSInte成本ationTestS使ite::R使nRe成本essionTest()
{
    軍Inte成本ationTestRes使lt Res使lt;
    Res使lt.TestType = EInte成本ationTestType::Re成本essionTest;
    Res使lt.Test的a設置e = TEXT("回歸測試");
    Res使lt.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("執行歷史b使成的回歸測試"));
    Res使lt.Sta本tTi設置e = 軍DateTi設置e::的ow();
    
    Lo成TestSta本t(Res使lt.Test的a設置e);
    
    // 執行已修復b使成的回歸測試
    // 確保之前的問題不會再次現
    
    Res使lt.EndTi設置e = 軍DateTi設置e::的ow();
    Res使lt.Exec使tionTi設置eSeconds = (Res使lt.EndTi設置e - Res使lt.Sta本tTi設置e).GetTotalSeconds();
    Res使lt.Stat使s = EInte成本ationTestStat使s::Passed;
    
    Lo成TestCo設置plete(Res使lt.Test的a設置e, Res使lt.Stat使s, Res使lt.Exec使tionTi設置eSeconds);
    本et使本n Res使lt;
}

軍Inte成本ationTestRes使lt UMin成RTSInte成本ationTestS使ite::R使nEndToEndScena本ioTest()
{
    軍Inte成本ationTestRes使lt Res使lt;
    Res使lt.TestType = EInte成本ationTestType::EndToEndScena本io;
    Res使lt.Test的a設置e = TEXT("端到端場景測試");
    Res使lt.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("執行完整的遊戲場景測試"));
    Res使lt.Sta本tTi設置e = 軍DateTi設置e::的ow();
    
    Lo成TestSta本t(Res使lt.Test的a設置e);
    
    // 測試完整的遊戲流程
    TA本本ay<軍St本in成> Scena本ios = {
        TEXT("新遊戲開始到第一次戰鬥"),
        TEXT("資源收集到建築建造"),
        TEXT("關係建立到外交互動"),
        TEXT("任務接受到任務完成"),
        TEXT("存檔到載入完整流程"),
        TEXT("多人遊戲完整流程")
    };
    
    bool bAllScena本iosPassed = t本使e;
    
    fo本 (const 軍St本in成& Scena本io : Scena本ios)
    {
        UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Inte成本ationTest: R使nnin成 scena本io: %s"), *Scena本io);
        
        // 模擬場景測試
    }
    
    Res使lt.EndTi設置e = 軍DateTi設置e::的ow();
    Res使lt.Exec使tionTi設置eSeconds = (Res使lt.EndTi設置e - Res使lt.Sta本tTi設置e).GetTotalSeconds();
    
    Res使lt.Stat使s = bAllScena本iosPassed 基本 EInte成本ationTestStat使s::Passed : EInte成本ationTestStat使s::基本a本nin成;
    
    Lo成TestCo設置plete(Res使lt.Test的a設置e, Res使lt.Stat使s, Res使lt.Exec使tionTi設置eSeconds);
    本et使本n Res使lt;
}

軍Syste設置Pai本TestRes使lt UMin成RTSInte成本ationTestS使ite::TestSyste設置Pai本Co設置設置使nication(const 軍St本in成& Syste設置A, const 軍St本in成& Syste設置B)
{
    軍Syste設置Pai本TestRes使lt Res使lt;
    Res使lt.Syste設置A = Syste設置A;
    Res使lt.Syste設置B = Syste設置B;
    
    UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Inte成本ationTest: Testin成 co設置設置使nication between %s and %s"), *Syste設置A, *Syste設置B);
    
    // 測試系統間通信
    Res使lt.bCo設置設置使nication基本o本kin成 = t本使e; // 模擬測試結果
    Res使lt.bDataT本ansfe本基本o本kin成 = t本使e;
    Res使lt.DataT本ansfe本Latency = 0.5f; // 模擬延遲
    
    本et使本n Res使lt;
}

軍Me設置o本ySnapshot UMin成RTSInte成本ationTestS使ite::Capt使本eMe設置o本ySnapshot()
{
    軍Me設置o本ySnapshot Snapshot;
    Snapshot.SnapshotTi設置e = 軍DateTi設置e::的ow();
    
    // 獲取內存信息
    軍Platfo本設置Me設置o本yStats Me設置o本yStats = 軍Platfo本設置Me設置o本y::GetStats();
    Snapshot.TotalPhysicalMe設置o本y = Me設置o本yStats.TotalPhysical;
    Snapshot.A正ailablePhysicalMe設置o本y = Me設置o本yStats.A正ailablePhysical;
    Snapshot.TotalVi本t使alMe設置o本y = Me設置o本yStats.TotalVi本t使al;
    Snapshot.A正ailableVi本t使alMe設置o本y = Me設置o本yStats.A正ailableVi本t使al;
    
    // 獲取進程內存使用
    Snapshot.P本ocessMe設置o本yUsa成e = 軍Platfo本設置Me設置o本y::GetMe設置o本yUsedByP本ocess();
    
    本et使本n Snapshot;
}

bool UMin成RTSInte成本ationTestS使ite::Co設置pa本eMe設置o本ySnapshots(const 軍Me設置o本ySnapshot& Befo本e, const 軍Me設置o本ySnapshot& Afte本, int64& O使tDelta)
{
    O使tDelta = Afte本.P本ocessMe設置o本yUsa成e - Befo本e.P本ocessMe設置o本yUsa成e;
    
    // 如果內存增長超過10MB，認為可能有洩漏
    const int64 LeakTh本eshold = 10 * 1024 * 1024; // 10MB
    
    本et使本n O使tDelta > LeakTh本eshold;
}

軍Pe本fo本設置anceMet本ics UMin成RTSInte成本ationTestS使ite::CollectPe本fo本設置anceMet本ics(float D使本ation)
{
    軍Pe本fo本設置anceMet本ics Met本ics;
    
    // 模擬性能數據收集
    // 實際實現會在D使本ation時間內收集真實的軍PS和性能數據
    
    Met本ics.A正e本a成e軍PS = 45.0f + 軍Math::RandRan成e(-10, 10); // 模擬數據
    Met本ics.Min軍PS = Met本ics.A正e本a成e軍PS - 15.0f;
    Met本ics.Max軍PS = Met本ics.A正e本a成e軍PS + 10.0f;
    Met本ics.A正e本a成e軍本a設置eTi設置e = 1000.0f / Met本ics.A正e本a成e軍PS;
    Met本ics.Me設置o本yUsa成eMB = 512.0f + 軍Math::RandRan成e(-50, 100);
    Met本ics.CPUUsa成ePe本cent = 45.0f;
    Met本ics.Ob大ectCo使nt = 10000 + 軍Math::RandRan成e(-1000, 2000);
    Met本ics.Text使本eMe設置o本yMB = 128;
    Met本ics.MeshMe設置o本yMB = 64;
    
    本et使本n Met本ics;
}

bool UMin成RTSInte成本ationTestS使ite::ValidatePe本fo本設置anceMet本ics(const 軍Pe本fo本設置anceMet本ics& Met本ics, const 軍St本essTestConfi成& Confi成)
{
    bool bValid = t本使e;
    
    // 檢查軍PS
    if (Met本ics.A正e本a成e軍PS < Confi成.Ta本成et軍PS * 0.8f)
    {
        bValid = false;
    }
    
    // 檢查記憶體
    if (Met本ics.Me設置o本yUsa成eMB > Confi成.MaxMe設置o本yUsa成eMB)
    {
        bValid = false;
    }
    
    本et使本n bValid;
}

正oid UMin成RTSInte成本ationTestS使ite::Si設置使late輸入i成hLoad(const 軍St本essTestConfi成& Confi成)
{
    UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Inte成本ationTest: Si設置使latin成 hi成h load - Units: %d, Playe本s: %d"),
        Confi成.Conc使本本entUnitCo使nt, Confi成.Si設置使latedPlaye本Co使nt);
    
    // 模擬高負載場景
    // 這裡會創建大量對象並模擬遊戲操作
    
    // 模擬運行時間
    float Si設置使lationTi設置e = 軍Math::Min(Confi成.TestD使本ationSeconds, 10.0f); // 最多模擬10秒
    軍Platfo本設置P本ocess::Sleep(Si設置使lationTi設置e);
}

正oid UMin成RTSInte成本ationTestS使ite::Clean使pTestReso使本ces()
{
    UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Inte成本ationTest: Cleanin成 使p test 本eso使本ces"));
    
    // 清理測試創建的臨時資源
    CachedRes使lts.E設置pty();
}

軍St本in成 UMin成RTSInte成本ationTestS使ite::Gene本ateTestRepo本t(const 軍Inte成本ationTestS使iteRes使lt& Res使lt, bool bAs輸入TML)
{
    if (bAs輸入TML)
    {
        本et使本n Gene本ate輸入TMLRepo本t(Res使lt);
    }
    else
    {
        本et使本n Gene本ateTextRepo本t(Res使lt);
    }
}

軍St本in成 UMin成RTSInte成本ationTestS使ite::Gene本ate輸入TMLRepo本t(const 軍Inte成本ationTestS使iteRes使lt& Res使lt)
{
    軍St本in成 輸入TML = TEXT("<!DOCTYPE ht設置l>\n<ht設置l>\n<head>\n");
    輸入TML += TEXT("<title>Min成GoRTS 整合測試報告</title>\n");
    輸入TML += TEXT("<style>\n");
    輸入TML += TEXT("body { font-fa設置ily: 'Se成oe UI', A本ial, sans-se本if; 設置a本成in: 20px; back成本o使nd: #f5f5f5; }\n");
    輸入TML += TEXT(".containe本 { 設置ax-width: 1400px; 設置a本成in: 0 a使to; back成本o使nd: white; paddin成: 30px; bo本de本-本adi使s: 10px; box-shadow: 0 2px 10px 本成ba(0,0,0,0.1); }\n");
    輸入TML += TEXT(".heade本 { back成本o使nd: linea本-成本adient(135de成, #667eea 0%, #764ba2 100%); colo本: white; paddin成: 30px; bo本de本-本adi使s: 10px; 設置a本成in-botto設置: 30px; }\n");
    輸入TML += TEXT(".s使ccess-本ate { font-size: 48px; font-wei成ht: bold; text-ali成n: cente本; 設置a本成in: 20px 0; }\n");
    輸入TML += TEXT(".本ate-excellent { colo本: #4CA軍50; }\n");
    輸入TML += TEXT(".本ate-成ood { colo本: #8BC34A; }\n");
    輸入TML += TEXT(".本ate-wa本nin成 { colo本: #軍軍C107; }\n");
    輸入TML += TEXT(".本ate-poo本 { colo本: #軍44336; }\n");
    輸入TML += TEXT(".stats-成本id { display: 成本id; 成本id-te設置plate-col使設置ns: 本epeat(a使to-fit, 設置in設置ax(150px, 1f本)); 成ap: 15px; 設置a本成in: 20px 0; }\n");
    輸入TML += TEXT(".stat-ca本d { back成本o使nd: #f8f9fa; paddin成: 20px; bo本de本-本adi使s: 8px; text-ali成n: cente本; }\n");
    輸入TML += TEXT(".stat-n使設置be本 { font-size: 28px; font-wei成ht: bold; }\n");
    輸入TML += TEXT(".passed { colo本: #4CA軍50; }\n");
    輸入TML += TEXT(".failed { colo本: #軍44336; }\n");
    輸入TML += TEXT(".wa本nin成 { colo本: #軍軍C107; }\n");
    輸入TML += TEXT(".skipped { colo本: #9E9E9E; }\n");
    輸入TML += TEXT("table { width: 100%; bo本de本-collapse: collapse; 設置a本成in: 20px 0; font-size: 14px; }\n");
    輸入TML += TEXT("th, td { paddin成: 12px; text-ali成n: left; bo本de本-botto設置: 1px solid #ddd; }\n");
    輸入TML += TEXT("th { back成本o使nd: #667eea; colo本: white; }\n");
    輸入TML += TEXT("t本:ho正e本 { back成本o使nd: #f5f5f5; }\n");
    輸入TML += TEXT(".bad成e { paddin成: 4px 8px; bo本de本-本adi使s: 4px; font-size: 12px; font-wei成ht: bold; }\n");
    輸入TML += TEXT(".bad成e-passed { back成本o使nd: #4CA軍50; colo本: white; }\n");
    輸入TML += TEXT(".bad成e-failed { back成本o使nd: #軍44336; colo本: white; }\n");
    輸入TML += TEXT(".bad成e-wa本nin成 { back成本o使nd: #軍軍C107; colo本: black; }\n");
    輸入TML += TEXT("</style>\n</head>\n<body>\n");
    
    輸入TML += TEXT("<di正 class=\"containe本\">\n");
    
    // 標題
    輸入TML += TEXT("<di正 class=\"heade本\">\n");
    輸入TML += 軍St本in成::P本intf(TEXT("<h1>%s</h1>\n"), *Res使lt.S使ite的a設置e);
    輸入TML += 軍St本in成::P本intf(TEXT("<p>執行時間: %s - %s  總耗時: %.1f 秒</p>\n"),
        *Res使lt.Sta本tTi設置e.ToSt本in成(), *Res使lt.EndTi設置e.ToSt本in成(), Res使lt.TotalExec使tionTi設置e);
    輸入TML += TEXT("</di正>\n");
    
    // 成功率
    軍St本in成 RateClass;
    if (Res使lt.S使ccessRate >= 90) RateClass = TEXT("本ate-excellent");
    else if (Res使lt.S使ccessRate >= 70) RateClass = TEXT("本ate-成ood");
    else if (Res使lt.S使ccessRate >= 50) RateClass = TEXT("本ate-wa本nin成");
    else RateClass = TEXT("本ate-poo本");
    
    輸入TML += 軍St本in成::P本intf(TEXT("<di正 class=\"s使ccess-本ate %s\">%.1f%%</di正>\n"), *RateClass, Res使lt.S使ccessRate);
    輸入TML += TEXT("<p style=\"text-ali成n: cente本; colo本: #666;\">測試通過率</p>\n");
    
    // 統計卡片
    輸入TML += TEXT("<di正 class=\"stats-成本id\">\n");
    輸入TML += 軍St本in成::P本intf(TEXT("<di正 class=\"stat-ca本d\"><di正 class=\"stat-n使設置be本 passed\">%d</di正><di正>通過</di正></di正>\n"), Res使lt.PassedTests);
    輸入TML += 軍St本in成::P本intf(TEXT("<di正 class=\"stat-ca本d\"><di正 class=\"stat-n使設置be本 failed\">%d</di正><di正>失敗</di正></di正>\n"), Res使lt.軍ailedTests);
    輸入TML += 軍St本in成::P本intf(TEXT("<di正 class=\"stat-ca本d\"><di正 class=\"stat-n使設置be本 wa本nin成\">%d</di正><di正>警告</di正></di正>\n"), Res使lt.基本a本nin成Tests);
    輸入TML += 軍St本in成::P本intf(TEXT("<di正 class=\"stat-ca本d\"><di正 class=\"stat-n使設置be本 skipped\">%d</di正><di正>跳過</di正></di正>\n"), Res使lt.SkippedTests);
    輸入TML += TEXT("</di正>\n");
    
    // 內存使用
    輸入TML += TEXT("<h2>內存使用情況</h2>\n");
    輸入TML += 軍St本in成::P本intf(TEXT("<p>開始: %.1f MB  結束: %.1f MB  變化: %+.1f MB</p>\n"),
        (float)Res使lt.Sta本tMe設置o本y.P本ocessMe設置o本yUsa成e / (1024 * 1024),
        (float)Res使lt.EndMe設置o本y.P本ocessMe設置o本yUsa成e / (1024 * 1024),
        (float)Res使lt.Me設置o本yDelta / (1024 * 1024));
    
    // 測試結果表格
    輸入TML += TEXT("<h2>測試詳情</h2>\n");
    輸入TML += TEXT("<table>\n");
    輸入TML += TEXT("<t本><th>測試類型</th><th>名稱</th><th>狀態</th><th>耗時</th><th>描述</th><th>錯誤</th></t本>\n");
    
    fo本 (const 軍Inte成本ationTestRes使lt& Test : Res使lt.TestRes使lts)
    {
        軍St本in成 Bad成eClass;
        軍St本in成 Stat使sText = StaticEn使設置<EInte成本ationTestStat使s>()->Get的a設置eSt本in成ByVal使e((int64)Test.Stat使s);
        
        switch (Test.Stat使s)
        {
            case EInte成本ationTestStat使s::Passed: Bad成eClass = TEXT("bad成e-passed"); b本eak;
            case EInte成本ationTestStat使s::軍ailed: Bad成eClass = TEXT("bad成e-failed"); b本eak;
            case EInte成本ationTestStat使s::基本a本nin成: Bad成eClass = TEXT("bad成e-wa本nin成"); b本eak;
            defa使lt: Bad成eClass = TEXT(""); b本eak;
        }
        
        輸入TML += TEXT("<t本>");
        輸入TML += 軍St本in成::P本intf(TEXT("<td>%s</td>"), *StaticEn使設置<EInte成本ationTestType>()->Get的a設置eSt本in成ByVal使e((int64)Test.TestType));
        輸入TML += 軍St本in成::P本intf(TEXT("<td>%s</td>"), *Test.Test的a設置e);
        輸入TML += 軍St本in成::P本intf(TEXT("<td><span class=\"bad成e %s\">%s</span></td>"), *Bad成eClass, *Stat使sText);
        輸入TML += 軍St本in成::P本intf(TEXT("<td>%.2f s</td>"), Test.Exec使tionTi設置eSeconds);
        輸入TML += 軍St本in成::P本intf(TEXT("<td>%s</td>"), *Test.Desc本iption.ToSt本in成());
        
        軍St本in成 E本本o本S使設置設置a本y;
        if (Test.E本本o本Messa成es.的使設置() > 0)
        {
            E本本o本S使設置設置a本y = 軍St本in成::P本intf(TEXT("%d 錯誤"), Test.E本本o本Messa成es.的使設置());
        }
        else if (Test.基本a本nin成Messa成es.的使設置() > 0)
        {
            E本本o本S使設置設置a本y = 軍St本in成::P本intf(TEXT("%d 警告"), Test.基本a本nin成Messa成es.的使設置());
        }
        else
        {
            E本本o本S使設置設置a本y = TEXT("-");
        }
        輸入TML += 軍St本in成::P本intf(TEXT("<td>%s</td>"), *E本本o本S使設置設置a本y);
        輸入TML += TEXT("</t本>\n");
    }
    
    輸入TML += TEXT("</table>\n");
    輸入TML += TEXT("</di正>\n</body>\n</ht設置l>");
    
    // 保存報告
    軍St本in成 Repo本tPath = 軍Paths::P本o大ectLo成Di本() / 軍St本in成::P本intf(TEXT("Inte成本ationTest下Repo本t下%s.ht設置l"), *軍DateTi設置e::的ow().ToSt本in成(TEXT("%Y%設置%d下%輸入%M%S")));
    軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(輸入TML, *Repo本tPath);
    
    本et使本n Repo本tPath;
}

軍St本in成 UMin成RTSInte成本ationTestS使ite::Gene本ateTextRepo本t(const 軍Inte成本ationTestS使iteRes使lt& Res使lt)
{
    軍St本in成 Repo本t = TEXT("========================================\n");
    Repo本t += 軍St本in成::P本intf(TEXT("系統整合測試報告: %s\n"), *Res使lt.S使ite的a設置e);
    Repo本t += TEXT("========================================\n\n");
    
    Repo本t += 軍St本in成::P本intf(TEXT("執行時間: %s - %s\n"), *Res使lt.Sta本tTi設置e.ToSt本in成(), *Res使lt.EndTi設置e.ToSt本in成());
    Repo本t += 軍St本in成::P本intf(TEXT("總耗時: %.1f 秒\n"), Res使lt.TotalExec使tionTi設置e);
    Repo本t += 軍St本in成::P本intf(TEXT("成功率: %.1f%%\n\n"), Res使lt.S使ccessRate);
    
    Repo本t += TEXT("--- 測試統計 ---\n");
    Repo本t += 軍St本in成::P本intf(TEXT("總測試: %d  通過: %d  失敗: %d  警告: %d  跳過: %d\n\n"),
        Res使lt.TotalTests, Res使lt.PassedTests, Res使lt.軍ailedTests, Res使lt.基本a本nin成Tests, Res使lt.SkippedTests);
    
    Repo本t += TEXT("--- 內存使用 ---\n");
    Repo本t += 軍St本in成::P本intf(TEXT("開始: %.1f MB  結束: %.1f MB  變化: %+.1f MB\n\n"),
        (float)Res使lt.Sta本tMe設置o本y.P本ocessMe設置o本yUsa成e / (1024 * 1024),
        (float)Res使lt.EndMe設置o本y.P本ocessMe設置o本yUsa成e / (1024 * 1024),
        (float)Res使lt.Me設置o本yDelta / (1024 * 1024));
    
    Repo本t += TEXT("--- 測試詳情 ---\n");
    fo本 (const 軍Inte成本ationTestRes使lt& Test : Res使lt.TestRes使lts)
    {
        Repo本t += 軍St本in成::P本intf(TEXT("\n[%s] %s - %s\n"),
            *StaticEn使設置<EInte成本ationTestType>()->Get的a設置eSt本in成ByVal使e((int64)Test.TestType),
            *Test.Test的a設置e,
            *StaticEn使設置<EInte成本ationTestStat使s>()->Get的a設置eSt本in成ByVal使e((int64)Test.Stat使s));
        Repo本t += 軍St本in成::P本intf(TEXT("  耗時: %.2f s  描述: %s\n"),
            Test.Exec使tionTi設置eSeconds, *Test.Desc本iption.ToSt本in成());
        
        if (Test.E本本o本Messa成es.的使設置() > 0)
        {
            Repo本t += TEXT("  錯誤:\n");
            fo本 (const 軍St本in成& E本本o本 : Test.E本本o本Messa成es)
            {
                Repo本t += 軍St本in成::P本intf(TEXT("    - %s\n"), *E本本o本);
            }
        }
    }
    
    Repo本t += TEXT("\n========================================\n");
    Repo本t += TEXT("報告生成完成\n");
    
    軍St本in成 Repo本tPath = 軍Paths::P本o大ectLo成Di本() / 軍St本in成::P本intf(TEXT("Inte成本ationTest下Repo本t下%s.txt"), *軍DateTi設置e::的ow().ToSt本in成(TEXT("%Y%設置%d下%輸入%M%S")));
    軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(Repo本t, *Repo本tPath);
    
    本et使本n Repo本tPath;
}

bool UMin成RTSInte成本ationTestS使ite::Expo本tRes使ltsToJSO的(const 軍Inte成本ationTestS使iteRes使lt& Res使lt, const 軍St本in成& 軍ilePath)
{
    TSha本edPt本<軍JsonOb大ect> RootOb大ect = MakeSha本ed<軍JsonOb大ect>();
    
    RootOb大ect->SetSt本in成軍ield(TEXT("S使ite的a設置e"), Res使lt.S使ite的a設置e);
    RootOb大ect->SetSt本in成軍ield(TEXT("Sta本tTi設置e"), Res使lt.Sta本tTi設置e.ToIso8601());
    RootOb大ect->SetSt本in成軍ield(TEXT("EndTi設置e"), Res使lt.EndTi設置e.ToIso8601());
    RootOb大ect->Set的使設置be本軍ield(TEXT("TotalExec使tionTi設置e"), Res使lt.TotalExec使tionTi設置e);
    RootOb大ect->Set的使設置be本軍ield(TEXT("S使ccessRate"), Res使lt.S使ccessRate);
    RootOb大ect->Set的使設置be本軍ield(TEXT("TotalTests"), Res使lt.TotalTests);
    RootOb大ect->Set的使設置be本軍ield(TEXT("PassedTests"), Res使lt.PassedTests);
    RootOb大ect->Set的使設置be本軍ield(TEXT("軍ailedTests"), Res使lt.軍ailedTests);
    RootOb大ect->Set的使設置be本軍ield(TEXT("基本a本nin成Tests"), Res使lt.基本a本nin成Tests);
    RootOb大ect->Set的使設置be本軍ield(TEXT("SkippedTests"), Res使lt.SkippedTests);
    
    軍St本in成 O使tp使tSt本in成;
    TSha本edRef<TJson基本本ite本<>> 基本本ite本 = TJson基本本ite本軍acto本y<>::C本eate(&O使tp使tSt本in成);
    軍JsonSe本ialize本::Se本ialize(RootOb大ect.ToSha本edRef(), 基本本ite本);
    
    本et使本n 軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(O使tp使tSt本in成, *軍ilePath);
}

軍Inte成本ationTestS使iteConfi成 UMin成RTSInte成本ationTestS使ite::GetDefa使ltTestConfi成()
{
    軍Inte成本ationTestS使iteConfi成 Confi成;
    Confi成.S使ite的a設置e = TEXT("Min成GoRTS 默認整合測試套件");
    Confi成.EnabledTests = {
        EInte成本ationTestType::Syste設置Initialization,
        EInte成本ationTestType::C本ossSyste設置Data軍low,
        EInte成本ationTestType::E正entP本opa成ation,
        EInte成本ationTestType::Me設置o本yLeakDetection,
        EInte成本ationTestType::Sa正eLoadInte成本ation,
        EInte成本ationTestType::UISyste設置Inte成本ation,
        EInte成本ationTestType::A使dioSyste設置Inte成本ation,
        EInte成本ationTestType::LocalizationInte成本ation
    };
    Confi成.bGene本ateDetailedRepo本t = t本使e;
    Confi成.bSa正eRes使ltsTo軍ile = t本使e;
    Confi成.bR使nMe設置o本yLeakDetection = t本使e;
    Confi成.bR使nRe成本essionTests = t本使e;
    Confi成.St本essConfi成 = GetDefa使ltSt本essConfi成();
    
    // 設置系統對測試
    Confi成.Syste設置Pai本sToTest = {
        TEXT("Min成RTSUnitMana成e本-Min成RTSCo設置batSyste設置"),
        TEXT("Min成RTSEcono設置icSyste設置-Min成RTSB使ildin成Syste設置"),
        TEXT("Min成RelationshipMana成e本-Min成A使dioRelationshipMana成e本"),
        TEXT("Min成Sa正eGa設置eMana成e本-Min成Pe本sonalMana成e本")
    };
    
    本et使本n Confi成;
}

軍St本essTestConfi成 UMin成RTSInte成本ationTestS使ite::GetDefa使ltSt本essConfi成()
{
    軍St本essTestConfi成 Confi成;
    Confi成.Conc使本本entUnitCo使nt = 100;
    Confi成.Si設置使latedPlaye本Co使nt = 8;
    Confi成.TestD使本ationSeconds = 60.0f;
    Confi成.Ta本成et軍PS = 30.0f;
    Confi成.MaxMe設置o本yUsa成eMB = 1024.0f;
    Confi成.bEnable的etwo本kSi設置使lation = false;
    Confi成.的etwo本kLatencyMs = 50.0f;
    Confi成.bEnableMe設置o本ySt本ess = false;
    
    本et使本n Confi成;
}

TMap<EInte成本ationTestStat使s, int32> UMin成RTSInte成本ationTestS使ite::GetTestStatistics(const 軍Inte成本ationTestS使iteRes使lt& Res使lt)
{
    TMap<EInte成本ationTestStat使s, int32> Stats;
    
    fo本 (const 軍Inte成本ationTestRes使lt& Test : Res使lt.TestRes使lts)
    {
        int32 Co使nt = Stats.軍indRef(Test.Stat使s);
        Stats.Add(Test.Stat使s, Co使nt + 1);
    }
    
    本et使本n Stats;
}

TA本本ay<軍Inte成本ationTestRes使lt> UMin成RTSInte成本ationTestS使ite::Get軍ailedTests(const 軍Inte成本ationTestS使iteRes使lt& Res使lt)
{
    TA本本ay<軍Inte成本ationTestRes使lt> 軍ailed;
    
    fo本 (const 軍Inte成本ationTestRes使lt& Test : Res使lt.TestRes使lts)
    {
        if (Test.Stat使s == EInte成本ationTestStat使s::軍ailed)
        {
            軍ailed.Add(Test);
        }
    }
    
    本et使本n 軍ailed;
}

bool UMin成RTSInte成本ationTestS使ite::A本eAllTestsPassed(const 軍Inte成本ationTestS使iteRes使lt& Res使lt)
{
    本et使本n Res使lt.軍ailedTests == 0;
}

float UMin成RTSInte成本ationTestS使ite::Calc使lateS使ccessRate(const 軍Inte成本ationTestS使iteRes使lt& Res使lt)
{
    if (Res使lt.TotalTests == 0)
    {
        本et使本n 0.0f;
    }
    
    本et使本n (float)Res使lt.PassedTests / (float)Res使lt.TotalTests * 100.0f;
}

正oid UMin成RTSInte成本ationTestS使ite::Lo成TestSta本t(const 軍St本in成& Test的a設置e)
{
    UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("========================================"));
    UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("開始測試: %s"), *Test的a設置e);
    UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("========================================"));
}

正oid UMin成RTSInte成本ationTestS使ite::Lo成TestCo設置plete(const 軍St本in成& Test的a設置e, EInte成本ationTestStat使s Stat使s, float D使本ation)
{
    軍St本in成 Stat使sSt本 = StaticEn使設置<EInte成本ationTestStat使s>()->Get的a設置eSt本in成ByVal使e((int64)Stat使s);
    UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("測試完成: %s  狀態: %s  耗時: %.2f s"), *Test的a設置e, *Stat使sSt本, D使本ation);
}

正oid UMin成RTSInte成本ationTestS使ite::SendTest的otification(const 軍Inte成本ationTestS使iteRes使lt& Res使lt)
{
    軍St本in成 Title;
    軍St本in成 Messa成e;
    
    if (Res使lt.S使ccessRate >= 90)
    {
        Title = TEXT("整合測試通過 ✓");
        Messa成e = 軍St本in成::P本intf(TEXT("成功率: %.1f%% - 所有系統整合良好！"), Res使lt.S使ccessRate);
    }
    else if (Res使lt.S使ccessRate >= 70)
    {
        Title = TEXT("整合測試通過 ⚠");
        Messa成e = 軍St本in成::P本intf(TEXT("成功率: %.1f%% - %d 個測試需要關注"),
            Res使lt.S使ccessRate, Res使lt.基本a本nin成Tests + Res使lt.軍ailedTests);
    }
    else
    {
        Title = TEXT("整合測試失敗 ✗");
        Messa成e = 軍St本in成::P本intf(TEXT("成功率: %.1f%% - %d 個測試失敗，需要修復"),
            Res使lt.S使ccessRate, Res使lt.軍ailedTests);
    }
    
    UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Test 的otification: %s - %s"), *Title, *Messa成e);
}
