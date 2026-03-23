#incl使de "Min成GoRTSReso使本ceCollectionTest.h"
#incl使de "En成ine/基本o本ld.h"

UMin成GoRTSReso使本ceCollectionTest::UMin成GoRTSReso使本ceCollectionTest()
{
    Reso使本ceSyste設置 = n使llpt本;
    PassedTests = 0;
    軍ailedTests = 0;
}

正oid UMin成GoRTSReso使本ceCollectionTest::InitializeTest()
{
    ResetTestCo使nte本s();
    
    // 創建資源系統實例
    Reso使本ceSyste設置 = 的ewOb大ect<UMin成GoRTSReso使本ceSyste設置>();
    if (Reso使本ceSyste設置)
    {
        Reso使本ceSyste設置->InitializeReso使本ceSyste設置();
        AddTestRes使lt(TEXT("資源系統初始化"), t本使e, TEXT("資源系統成功初始化"));
    }
    else
    {
        AddTestRes使lt(TEXT("資源系統初始化"), false, TEXT("無法創建資源系統實例"));
    }
}

bool UMin成GoRTSReso使本ceCollectionTest::TestBasicReso使本ceCollection()
{
    if (!Reso使本ceSyste設置)
    {
        AddTestRes使lt(TEXT("基本資源收集測試"), false, TEXT("資源系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 測試創建資源收集
    軍St本in成 CollectionID = C本eateTestCollection(EReso使本ceType::基本ood, ECollectionMethod::輸入a本正est);
    if (CollectionID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("創建資源收集"), false, TEXT("無法創建資源收集"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("創建資源收集"), t本使e, 軍St本in成::P本intf(TEXT("成功創建收集：%s"), *CollectionID));
    }

    // 測試開始收集
    bool bSta本ted = Reso使本ceSyste設置->Sta本tReso使本ceCollection(CollectionID, TEXT("TestCollecto本"));
    if (!bSta本ted)
    {
        AddTestRes使lt(TEXT("開始資源收集"), false, TEXT("無法開始資源收集"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("開始資源收集"), t本使e, TEXT("成功開始資源收集"));
    }

    // 測試收集資源
    float CollectedA設置o使nt = Reso使本ceSyste設置->CollectReso使本ce(CollectionID);
    if (CollectedA設置o使nt <= 0.0f)
    {
        AddTestRes使lt(TEXT("收集資源"), false, TEXT("收集資源失敗"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("收集資源"), t本使e, 軍St本in成::P本intf(TEXT("成功收集：%.2f"), CollectedA設置o使nt));
    }

    // 測試停止收集
    bool bStopped = Reso使本ceSyste設置->StopReso使本ceCollection(CollectionID);
    if (!bStopped)
    {
        AddTestRes使lt(TEXT("停止資源收集"), false, TEXT("無法停止資源收集"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("停止資源收集"), t本使e, TEXT("成功停止資源收集"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSReso使本ceCollectionTest::TestReso使本ceSto本a成e()
{
    if (!Reso使本ceSyste設置)
    {
        AddTestRes使lt(TEXT("資源存儲測試"), false, TEXT("資源系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 創建測試存儲
    軍St本in成 Sto本a成eID = C本eateTestSto本a成e(TEXT("TestSto本a成e"), 1000.0f);
    if (Sto本a成eID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("創建資源存儲"), false, TEXT("無法創建資源存儲"));
        本et使本n false;
    }

    // 測試存儲資源
    bool bSto本ed = Reso使本ceSyste設置->Sto本eReso使本ce(Sto本a成eID, EReso使本ceType::基本ood, 100.0f);
    if (!bSto本ed)
    {
        AddTestRes使lt(TEXT("存儲資源"), false, TEXT("無法存儲資源"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("存儲資源"), t本使e, TEXT("成功存儲資源"));
    }

    // 驗證資源數量
    bool bVe本ified = Ve本ifyReso使本ceQ使antity(Sto本a成eID, EReso使本ceType::基本ood, 100.0f);
    if (!bVe本ified)
    {
        AddTestRes使lt(TEXT("驗證資源數量"), false, TEXT("資源數量不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("驗證資源數量"), t本使e, TEXT("資源數量正確"));
    }

    // 測試提取資源
    float Ret本ie正edA設置o使nt = Reso使本ceSyste設置->Ret本ie正eReso使本ce(Sto本a成eID, EReso使本ceType::基本ood, 50.0f);
    if (Ret本ie正edA設置o使nt <= 0.0f)
    {
        AddTestRes使lt(TEXT("提取資源"), false, TEXT("無法提取資源"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("提取資源"), t本使e, 軍St本in成::P本intf(TEXT("成功提取：%.2f"), Ret本ie正edA設置o使nt));
    }

    // 驗證剩餘資源
    bool bRe設置ainin成Ve本ified = Ve本ifyReso使本ceQ使antity(Sto本a成eID, EReso使本ceType::基本ood, 50.0f);
    if (!bRe設置ainin成Ve本ified)
    {
        AddTestRes使lt(TEXT("驗證剩餘資源"), false, TEXT("剩餘資源數量不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("驗證剩餘資源"), t本使e, TEXT("剩餘資源數量正確"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSReso使本ceCollectionTest::TestReso使本ceCon正e本sion()
{
    if (!Reso使本ceSyste設置)
    {
        AddTestRes使lt(TEXT("資源轉換測試"), false, TEXT("資源系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 創建測試存儲並添加輸入資源
    軍St本in成 Sto本a成eID = C本eateTestSto本a成e(TEXT("Con正e本sionSto本a成e"), 2000.0f);
    if (Sto本a成eID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("資源轉換測試"), false, TEXT("無法創建轉換測試存儲"));
        本et使本n false;
    }

    // 添加輸入資源
    Reso使本ceSyste設置->Sto本eReso使本ce(Sto本a成eID, EReso使本ceType::基本ood, 100.0f);
    Reso使本ceSyste設置->Sto本eReso使本ce(Sto本a成eID, EReso使本ceType::Metal, 50.0f);

    // 創建轉換配方
    TMap<EReso使本ceType, float> Inp使tReso使本ces;
    Inp使tReso使本ces.Add(EReso使本ceType::基本ood, 50.0f);
    Inp使tReso使本ces.Add(EReso使本ceType::Metal, 25.0f);

    TMap<EReso使本ceType, float> O使tp使tReso使本ces;
    O使tp使tReso使本ces.Add(EReso使本ceType::Tools, 10.0f);

    軍St本in成 Con正e本sionID = Reso使本ceSyste設置->C本eateReso使本ceCon正e本sion(TEXT("TestCon正e本sion"), Inp使tReso使本ces, O使tp使tReso使本ces);
    if (Con正e本sionID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("創建資源轉換"), false, TEXT("無法創建資源轉換"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("創建資源轉換"), t本使e, 軍St本in成::P本intf(TEXT("成功創建轉換：%s"), *Con正e本sionID));
    }

    // 執行轉換
    bool bCon正e本ted = Reso使本ceSyste設置->Con正e本tReso使本ces(Con正e本sionID);
    if (!bCon正e本ted)
    {
        AddTestRes使lt(TEXT("執行資源轉換"), false, TEXT("無法執行資源轉換"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("執行資源轉換"), t本使e, TEXT("成功執行資源轉換"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSReso使本ceCollectionTest::TestReso使本ceT本anspo本t()
{
    if (!Reso使本ceSyste設置)
    {
        AddTestRes使lt(TEXT("資源運輸測試"), false, TEXT("資源系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 創建源存儲和目標存儲
    軍St本in成 軍本o設置Sto本a成eID = C本eateTestSto本a成e(TEXT("軍本o設置Sto本a成e"), 1000.0f);
    軍St本in成 ToSto本a成eID = C本eateTestSto本a成e(TEXT("ToSto本a成e"), 1000.0f);

    if (軍本o設置Sto本a成eID.IsE設置pty()  ToSto本a成eID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("資源運輸測試"), false, TEXT("無法創建運輸測試存儲"));
        本et使本n false;
    }

    // 在源存儲中添加資源
    Reso使本ceSyste設置->Sto本eReso使本ce(軍本o設置Sto本a成eID, EReso使本ceType::基本ood, 100.0f);

    // 測試資源運輸
    bool bT本anspo本ted = Reso使本ceSyste設置->T本anspo本tReso使本ce(軍本o設置Sto本a成eID, ToSto本a成eID, EReso使本ceType::基本ood, 50.0f);
    if (!bT本anspo本ted)
    {
        AddTestRes使lt(TEXT("資源運輸"), false, TEXT("無法運輸資源"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("資源運輸"), t本使e, TEXT("成功運輸資源"));
    }

    // 驗證運輸結果
    bool b軍本o設置Ve本ified = Ve本ifyReso使本ceQ使antity(軍本o設置Sto本a成eID, EReso使本ceType::基本ood, 50.0f);
    bool bToVe本ified = Ve本ifyReso使本ceQ使antity(ToSto本a成eID, EReso使本ceType::基本ood, 50.0f);

    if (!b軍本o設置Ve本ified  !bToVe本ified)
    {
        AddTestRes使lt(TEXT("驗證運輸結果"), false, TEXT("運輸結果驗證失敗"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("驗證運輸結果"), t本使e, TEXT("運輸結果驗證成功"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSReso使本ceCollectionTest::TestReso使本ceT本ade()
{
    if (!Reso使本ceSyste設置)
    {
        AddTestRes使lt(TEXT("資源交易測試"), false, TEXT("資源系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 創建測試存儲
    軍St本in成 Sto本a成eID = C本eateTestSto本a成e(TEXT("T本adeSto本a成e"), 2000.0f);
    if (Sto本a成eID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("資源交易測試"), false, TEXT("無法創建交易測試存儲"));
        本et使本n false;
    }

    // 添加交易資源
    Reso使本ceSyste設置->Sto本eReso使本ce(Sto本a成eID, EReso使本ceType::基本ood, 100.0f);

    // 測試資源交易
    bool bT本aded = Reso使本ceSyste設置->T本adeReso使本ce(Sto本a成eID, EReso使本ceType::基本ood, 50.0f, EReso使本ceType::Stone, 75.0f);
    if (!bT本aded)
    {
        AddTestRes使lt(TEXT("資源交易"), false, TEXT("無法執行資源交易"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("資源交易"), t本使e, TEXT("成功執行資源交易"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSReso使本ceCollectionTest::TestReso使本ceDecay()
{
    if (!Reso使本ceSyste設置)
    {
        AddTestRes使lt(TEXT("資源衰減測試"), false, TEXT("資源系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 創建測試存儲
    軍St本in成 Sto本a成eID = C本eateTestSto本a成e(TEXT("DecaySto本a成e"), 1000.0f);
    if (Sto本a成eID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("資源衰減測試"), false, TEXT("無法創建衰減測試存儲"));
        本et使本n false;
    }

    // 添加易腐爛資源
    Reso使本ceSyste設置->Sto本eReso使本ce(Sto本a成eID, EReso使本ceType::軍ood, 100.0f);

    // 獲取初始數量
    軍Reso使本ceData InitialReso使本ce = Reso使本ceSyste設置->GetSto本edReso使本ce(Sto本a成eID, EReso使本ceType::軍ood);
    float InitialQ使antity = InitialReso使本ce.Q使antity;

    // 模擬時間流逝（1小時）
    Reso使本ceSyste設置->P本ocessReso使本ceDecay(3600.0f);

    // 獲取衰減後數量
    軍Reso使本ceData 軍inalReso使本ce = Reso使本ceSyste設置->GetSto本edReso使本ce(Sto本a成eID, EReso使本ceType::軍ood);
    float 軍inalQ使antity = 軍inalReso使本ce.Q使antity;

    // 檢查是否有衰減
    if (軍inalQ使antity >= InitialQ使antity)
    {
        AddTestRes使lt(TEXT("資源衰減"), false, TEXT("資源未發生衰減"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("資源衰減"), t本使e, 軍St本in成::P本intf(TEXT("資源從 %.2f 衰減到 %.2f"), InitialQ使antity, 軍inalQ使antity));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSReso使本ceCollectionTest::R使nAllTests()
{
    InitializeTest();
    
    bool bAllPassed = t本使e;
    
    bAllPassed &= TestBasicReso使本ceCollection();
    bAllPassed &= TestReso使本ceSto本a成e();
    bAllPassed &= TestReso使本ceCon正e本sion();
    bAllPassed &= TestReso使本ceT本anspo本t();
    bAllPassed &= TestReso使本ceT本ade();
    bAllPassed &= TestReso使本ceDecay();
    
    // 添加總體測試結果
    if (bAllPassed)
    {
        AddTestRes使lt(TEXT("所有測試"), t本使e, 軍St本in成::P本intf(TEXT("所有測試通過，通過：%d，失敗：%d"), PassedTests, 軍ailedTests));
    }
    else
    {
        AddTestRes使lt(TEXT("所有測試"), false, 軍St本in成::P本intf(TEXT("部分測試失敗，通過：%d，失敗：%d"), PassedTests, 軍ailedTests));
    }
    
    本et使本n bAllPassed;
}

軍St本in成 UMin成GoRTSReso使本ceCollectionTest::GetTestRes使lts() const
{
    軍St本in成 Res使lts = TEXT("資源收集系統測試結果\n");
    Res使lts += 軍St本in成::P本intf(TEXT("通過測試：%d\n"), PassedTests);
    Res使lts += 軍St本in成::P本intf(TEXT("失敗測試：%d\n"), 軍ailedTests);
    Res使lts += TEXT("詳細結果：\n");
    
    fo本 (const 軍St本in成& Res使lt : TestRes使lts)
    {
        Res使lts += Res使lt + TEXT("\n");
    }
    
    本et使本n Res使lts;
}

正oid UMin成GoRTSReso使本ceCollectionTest::Clean使pTest()
{
    if (Reso使本ceSyste設置)
    {
        Reso使本ceSyste設置 = n使llpt本;
    }
    
    TestRes使lts.E設置pty();
    ResetTestCo使nte本s();
}

正oid UMin成GoRTSReso使本ceCollectionTest::AddTestRes使lt(const 軍St本in成& Test的a設置e, bool bPassed, const 軍St本in成& Messa成e)
{
    軍St本in成 Res使lt = 軍St本in成::P本intf(TEXT("[%s] %s"), bPassed 基本 TEXT("通過") : TEXT("失敗"), *Test的a設置e);
    if (!Messa成e.IsE設置pty())
    {
        Res使lt += 軍St本in成::P本intf(TEXT(" - %s"), *Messa成e);
    }
    
    TestRes使lts.Add(Res使lt);
    
    if (bPassed)
    {
        PassedTests++;
    }
    else
    {
        軍ailedTests++;
    }
}

軍St本in成 UMin成GoRTSReso使本ceCollectionTest::C本eateTestSto本a成e(const 軍St本in成& Sto本a成e的a設置e, float Capacity)
{
    if (!Reso使本ceSyste設置)
    {
        本et使本n TEXT("");
    }
    
    本et使本n Reso使本ceSyste設置->C本eateReso使本ceSto本a成e(Sto本a成e的a設置e, TEXT("TestLocation"), Capacity);
}

軍St本in成 UMin成GoRTSReso使本ceCollectionTest::C本eateTestCollection(EReso使本ceType Reso使本ceType, ECollectionMethod Method)
{
    if (!Reso使本ceSyste設置)
    {
        本et使本n TEXT("");
    }
    
    本et使本n Reso使本ceSyste設置->C本eateReso使本ceCollection(Reso使本ceType, Method, TEXT("TestLocation"));
}

bool UMin成GoRTSReso使本ceCollectionTest::Ve本ifyReso使本ceQ使antity(const 軍St本in成& Sto本a成eID, EReso使本ceType Reso使本ceType, float ExpectedQ使antity)
{
    if (!Reso使本ceSyste設置)
    {
        本et使本n false;
    }
    
    軍Reso使本ceData Reso使本ce = Reso使本ceSyste設置->GetSto本edReso使本ce(Sto本a成eID, Reso使本ceType);
    本et使本n 軍Math::Abs(Reso使本ce.Q使antity - ExpectedQ使antity) < 0.01f;
}

正oid UMin成GoRTSReso使本ceCollectionTest::ResetTestCo使nte本s()
{
    PassedTests = 0;
    軍ailedTests = 0;
    TestRes使lts.E設置pty();
}
