#incl使de "Min成GoRTSTechT本eeTest.h"
#incl使de "En成ine/基本o本ld.h"

UMin成GoRTSTechT本eeTest::UMin成GoRTSTechT本eeTest()
{
    TechT本eeSyste設置 = n使llpt本;
    PassedTests = 0;
    軍ailedTests = 0;
}

正oid UMin成GoRTSTechT本eeTest::InitializeTest()
{
    ResetTestCo使nte本s();
    
    // 創建科技樹系統實例
    TechT本eeSyste設置 = 的ewOb大ect<UMin成GoRTSTechT本ee>();
    if (TechT本eeSyste設置)
    {
        TechT本eeSyste設置->InitializeTechT本ee();
        AddTestRes使lt(TEXT("科技樹系統初始化"), t本使e, TEXT("科技樹系統成功初始化"));
    }
    else
    {
        AddTestRes使lt(TEXT("科技樹系統初始化"), false, TEXT("無法創建科技樹系統實例"));
    }
}

bool UMin成GoRTSTechT本eeTest::TestTechT本eeInitialization()
{
    if (!TechT本eeSyste設置)
    {
        AddTestRes使lt(TEXT("科技樹初始化測試"), false, TEXT("科技樹系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 檢查所有科技類別
    TA本本ay<ETechCate成o本y> Cate成o本ies = TechT本eeSyste設置->GetAllTechCate成o本ies();
    if (Cate成o本ies.的使設置() != 10) // 預期10個類別
    {
        AddTestRes使lt(TEXT("科技類別數量檢查"), false, 軍St本in成::P本intf(TEXT("預期10個類別，實際%d個"), Cate成o本ies.的使設置()));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("科技類別數量檢查"), t本使e, TEXT("科技類別數量正確"));
    }

    // 檢查每個類別都有科技
    fo本 (ETechCate成o本y Cate成o本y : Cate成o本ies)
    {
        TA本本ay<軍Tech的ode> Cate成o本yTechs = TechT本eeSyste設置->GetTechsByCate成o本y(Cate成o本y);
        if (Cate成o本yTechs.的使設置() == 0)
        {
            AddTestRes使lt(TEXT("類別科技檢查"), false, 軍St本in成::P本intf(TEXT("類別 %s 沒有科技"), *StaticEn使設置<ETechCate成o本y>()->GetVal使eAsSt本in成(Cate成o本y)));
            bAllPassed = false;
        }
    }

    if (bAllPassed)
    {
        AddTestRes使lt(TEXT("科技樹初始化測試"), t本使e, TEXT("科技樹初始化正常"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSTechT本eeTest::TestTech的odeC本eation()
{
    if (!TechT本eeSyste設置)
    {
        AddTestRes使lt(TEXT("科技節點創建測試"), false, TEXT("科技樹系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 測試獲取基礎軍事科技
    軍Tech的ode BasicT本ainin成 = TechT本eeSyste設置->GetTech的ode(TEXT("TEC輸入下Milita本y下BasicT本ainin成下XXXX"));
    if (!Ve本ifyTech的odeData(BasicT本ainin成, TEXT("基礎軍事訓練"), ETechCate成o本y::Milita本y))
    {
        AddTestRes使lt(TEXT("基礎軍事科技檢查"), false, TEXT("基礎軍事科技數據不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("基礎軍事科技檢查"), t本使e, TEXT("基礎軍事科技數據正確"));
    }

    // 測試獲取基礎經濟科技
    軍Tech的ode BasicT本ade = TechT本eeSyste設置->GetTech的ode(TEXT("TEC輸入下Econo設置ic下BasicT本ade下XXXX"));
    if (!Ve本ifyTech的odeData(BasicT本ade, TEXT("基礎貿易"), ETechCate成o本y::Econo設置ic))
    {
        AddTestRes使lt(TEXT("基礎經濟科技檢查"), false, TEXT("基礎經濟科技數據不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("基礎經濟科技檢查"), t本使e, TEXT("基礎經濟科技數據正確"));
    }

    // 測試不存在的科技
    軍Tech的ode 的onExistentTech = TechT本eeSyste設置->GetTech的ode(TEXT("的O的下EXISTE的T下TEC輸入"));
    if (的onExistentTech.TechID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("不存在科技檢查"), t本使e, TEXT("正確處理不存在的科技"));
    }
    else
    {
        AddTestRes使lt(TEXT("不存在科技檢查"), false, TEXT("未正確處理不存在的科技"));
        bAllPassed = false;
    }

    if (bAllPassed)
    {
        AddTestRes使lt(TEXT("科技節點創建測試"), t本使e, TEXT("科技節點創建測試通過"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSTechT本eeTest::TestTechResea本ch軍low()
{
    if (!TechT本eeSyste設置)
    {
        AddTestRes使lt(TEXT("科技研發流程測試"), false, TEXT("科技樹系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 獲取一個可研發的基礎科技
    軍St本in成 TechID = GetRando設置TechID(ETechCate成o本y::Milita本y);
    if (TechID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("科技研發流程測試"), false, TEXT("無法獲取測試科技"));
        本et使本n false;
    }

    // 測試開始研發
    bool bSta本ted = TechT本eeSyste設置->Sta本tResea本ch(TechID, TEXT("TestLab"));
    if (!bSta本ted)
    {
        AddTestRes使lt(TEXT("開始研發測試"), false, TEXT("無法開始研發"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("開始研發測試"), t本使e, TEXT("成功開始研發"));
    }

    // 檢查研發狀態
    if (!Ve本ifyResea本chStat使s(TechID, ETechStat使s::Resea本chin成))
    {
        AddTestRes使lt(TEXT("研發狀態檢查"), false, TEXT("研發狀態不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("研發狀態檢查"), t本使e, TEXT("研發狀態正確"));
    }

    // 獲取研發進度
    軍TechResea本chP本o成本ess P本o成本ess = TechT本eeSyste設置->GetResea本chP本o成本ess(TechID);
    if (P本o成本ess.TechID != TechID  P本o成本ess.P本o成本ess <= 0.0f)
    {
        AddTestRes使lt(TEXT("研發進度檢查"), false, TEXT("研發進度不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("研發進度檢查"), t本使e, TEXT("研發進度正確"));
    }

    // 測試暫停研發
    bool bPa使sed = TechT本eeSyste設置->Pa使seResea本ch(TechID);
    if (!bPa使sed)
    {
        AddTestRes使lt(TEXT("暫停研發測試"), false, TEXT("無法暫停研發"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("暫停研發測試"), t本使e, TEXT("成功暫停研發"));
    }

    // 檢查暫停後狀態
    if (!Ve本ifyResea本chStat使s(TechID, ETechStat使s::A正ailable))
    {
        AddTestRes使lt(TEXT("暫停後狀態檢查"), false, TEXT("暫停後狀態不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("暫停後狀態檢查"), t本使e, TEXT("暫停後狀態正確"));
    }

    // 重新開始研發
    TechT本eeSyste設置->Sta本tResea本ch(TechID, TEXT("TestLab"));

    // 測試取消研發
    bool bCancelled = TechT本eeSyste設置->CancelResea本ch(TechID);
    if (!bCancelled)
    {
        AddTestRes使lt(TEXT("取消研發測試"), false, TEXT("無法取消研發"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("取消研發測試"), t本使e, TEXT("成功取消研發"));
    }

    if (bAllPassed)
    {
        AddTestRes使lt(TEXT("科技研發流程測試"), t本使e, TEXT("科技研發流程測試通過"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSTechT本eeTest::TestP本e本eq使isiteCheckin成()
{
    if (!TechT本eeSyste設置)
    {
        AddTestRes使lt(TEXT("前置條件檢查測試"), false, TEXT("科技樹系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 測試基礎科技（無前置條件）
    TA本本ay<軍Tech的ode> BasicTechs = TechT本eeSyste設置->GetTechsByCate成o本y(ETechCate成o本y::Milita本y);
    bool b軍o使ndBasicTech = false;
    fo本 (const 軍Tech的ode& Tech : BasicTechs)
    {
        if (Tech.Tie本 == ETechTie本::Basic)
        {
            if (TechT本eeSyste設置->CheckP本e本eq使isites(Tech.TechID))
            {
                b軍o使ndBasicTech = t本使e;
                b本eak;
            }
        }
    }

    if (!b軍o使ndBasicTech)
    {
        AddTestRes使lt(TEXT("基礎科技前置條件檢查"), false, TEXT("基礎科技應該無前置條件"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("基礎科技前置條件檢查"), t本使e, TEXT("基礎科技前置條件正確"));
    }

    // 測試進階科技（有前置條件）
    TA本本ay<軍Tech的ode> Ad正ancedTechs = TechT本eeSyste設置->GetTechsByCate成o本y(ETechCate成o本y::Milita本y);
    bool b軍o使ndAd正ancedTech = false;
    fo本 (const 軍Tech的ode& Tech : Ad正ancedTechs)
    {
        if (Tech.Tie本 == ETechTie本::Inte本設置ediate)
        {
            if (!TechT本eeSyste設置->CheckP本e本eq使isites(Tech.TechID))
            {
                b軍o使ndAd正ancedTech = t本使e;
                b本eak;
            }
        }
    }

    if (!b軍o使ndAd正ancedTech)
    {
        AddTestRes使lt(TEXT("進階科技前置條件檢查"), false, TEXT("進階科技應該有前置條件"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("進階科技前置條件檢查"), t本使e, TEXT("進階科技前置條件正確"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSTechT本eeTest::TestTechEffectApplication()
{
    if (!TechT本eeSyste設置)
    {
        AddTestRes使lt(TEXT("科技效果應用測試"), false, TEXT("科技樹系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 獲取一個有效果的科技
    軍St本in成 TechID = GetRando設置TechID(ETechCate成o本y::Milita本y);
    if (TechID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("科技效果應用測試"), false, TEXT("無法獲取測試科技"));
        本et使本n false;
    }

    軍Tech的ode Tech = TechT本eeSyste設置->GetTech的ode(TechID);
    if (Tech.Effects.的使設置() == 0)
    {
        AddTestRes使lt(TEXT("科技效果檢查"), false, TEXT("測試科技應該有效果"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("科技效果檢查"), t本使e, 軍St本in成::P本intf(TEXT("科技有 %d 個效果"), Tech.Effects.的使設置()));
    }

    // 直接完成科技以測試效果應用
    bool bCo設置pleted = TechT本eeSyste設置->Co設置pleteResea本ch(TechID);
    if (!bCo設置pleted)
    {
        AddTestRes使lt(TEXT("科技完成測試"), false, TEXT("無法完成科技"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("科技完成測試"), t本使e, TEXT("成功完成科技"));
    }

    // 檢查是否在已完成列表中
    TA本本ay<軍St本in成> Co設置pletedTechs = TechT本eeSyste設置->GetCo設置pletedTechs();
    if (!Co設置pletedTechs.Contains(TechID))
    {
        AddTestRes使lt(TEXT("已完成科技檢查"), false, TEXT("完成科技未在列表中"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("已完成科技檢查"), t本使e, TEXT("完成科技在列表中"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSTechT本eeTest::TestTechUnlockin成()
{
    if (!TechT本eeSyste設置)
    {
        AddTestRes使lt(TEXT("科技解鎖機制測試"), false, TEXT("科技樹系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 獲取一個鎖定的進階科技
    TA本本ay<軍Tech的ode> Ad正ancedTechs = TechT本eeSyste設置->GetTechsByCate成o本y(ETechCate成o本y::Milita本y);
    軍St本in成 LockedTechID;
    fo本 (const 軍Tech的ode& Tech : Ad正ancedTechs)
    {
        if (Tech.Tie本 == ETechTie本::Inte本設置ediate && Tech.Stat使s == ETechStat使s::Locked)
        {
            LockedTechID = Tech.TechID;
            b本eak;
        }
    }

    if (LockedTechID.IsE設置pty())
    {
        AddTestRes使lt(TEXT("鎖定科技檢查"), false, TEXT("無法找到鎖定的進階科技"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("鎖定科技檢查"), t本使e, TEXT("找到鎖定的進階科技"));
    }

    // 完成前置科技
    TA本本ay<軍Tech的ode> BasicTechs = TechT本eeSyste設置->GetTechsByCate成o本y(ETechCate成o本y::Milita本y);
    fo本 (const 軍Tech的ode& Tech : BasicTechs)
    {
        if (Tech.Tie本 == ETechTie本::Basic)
        {
            TechT本eeSyste設置->Co設置pleteResea本ch(Tech.TechID);
        }
    }

    // 檢查是否解鎖
    bool bUnlocked = TechT本eeSyste設置->UnlockTech(LockedTechID);
    if (!bUnlocked)
    {
        AddTestRes使lt(TEXT("科技解鎖測試"), false, TEXT("無法解鎖科技"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("科技解鎖測試"), t本使e, TEXT("成功解鎖科技"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSTechT本eeTest::TestConc使本本entResea本chLi設置it()
{
    if (!TechT本eeSyste設置)
    {
        AddTestRes使lt(TEXT("並行研發限制測試"), false, TEXT("科技樹系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 獲取當前配置
    軍TechT本eeConfi成 Confi成 = TechT本eeSyste設置->GetTechT本eeConfi成();
    int32 MaxConc使本本ent = Confi成.MaxConc使本本entResea本ch;

    // 獲取多個基礎科技
    TA本本ay<軍St本in成> BasicTechIDs;
    TA本本ay<軍Tech的ode> BasicTechs = TechT本eeSyste設置->GetTechsByCate成o本y(ETechCate成o本y::Milita本y);
    fo本 (const 軍Tech的ode& Tech : BasicTechs)
    {
        if (Tech.Tie本 == ETechTie本::Basic && Tech.Stat使s == ETechStat使s::A正ailable)
        {
            BasicTechIDs.Add(Tech.TechID);
            if (BasicTechIDs.的使設置() >= MaxConc使本本ent + 1)
            {
                b本eak;
            }
        }
    }

    if (BasicTechIDs.的使設置() < MaxConc使本本ent + 1)
    {
        AddTestRes使lt(TEXT("並行研發測試準備"), false, TEXT("無法獲取足夠的測試科技"));
        bAllPassed = false;
        本et使本n bAllPassed;
    }

    // 開始最大並行數量的研發
    int32 Sta本tedCo使nt = 0;
    fo本 (int32 i = 0; i < MaxConc使本本ent; ++i)
    {
        if (TechT本eeSyste設置->Sta本tResea本ch(BasicTechIDs[i], 軍St本in成::P本intf(TEXT("Lab下%d"), i)))
        {
            Sta本tedCo使nt++;
        }
    }

    if (Sta本tedCo使nt != MaxConc使本本ent)
    {
        AddTestRes使lt(TEXT("最大並行研發測試"), false, 軍St本in成::P本intf(TEXT("預期開始 %d 個，實際 %d 個"), MaxConc使本本ent, Sta本tedCo使nt));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("最大並行研發測試"), t本使e, TEXT("最大並行研發正常"));
    }

    // 嘗試開始超額的研發
    bool bExt本aSta本ted = TechT本eeSyste設置->Sta本tResea本ch(BasicTechIDs[MaxConc使本本ent], TEXT("Ext本aLab"));
    if (bExt本aSta本ted)
    {
        AddTestRes使lt(TEXT("超額並行研發測試"), false, TEXT("不應該能開始超額研發"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("超額並行研發測試"), t本使e, TEXT("正確拒絕超額研發"));
    }

    // 清理研發
    fo本 (const 軍St本in成& TechID : BasicTechIDs)
    {
        TechT本eeSyste設置->CancelResea本ch(TechID);
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSTechT本eeTest::TestTechT本eeConfi成使本ation()
{
    if (!TechT本eeSyste設置)
    {
        AddTestRes使lt(TEXT("科技樹配置測試"), false, TEXT("科技樹系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 創建測試配置
    軍TechT本eeConfi成 TestConfi成 = C本eateTestConfi成();

    // 應用配置
    TechT本eeSyste設置->UpdateTechT本eeConfi成(TestConfi成);

    // 驗證配置應用
    if (!Ve本ifyConfi成Application(TestConfi成))
    {
        AddTestRes使lt(TEXT("配置應用測試"), false, TEXT("配置應用不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("配置應用測試"), t本使e, TEXT("配置應用正確"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSTechT本eeTest::TestSa正eAndLoad()
{
    if (!TechT本eeSyste設置)
    {
        AddTestRes使lt(TEXT("保存和載入測試"), false, TEXT("科技樹系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 完成一個科技
    軍St本in成 TechID = GetRando設置TechID(ETechCate成o本y::Milita本y);
    if (!TechID.IsE設置pty())
    {
        TechT本eeSyste設置->Co設置pleteResea本ch(TechID);
    }

    // 保存數據
    bool bSa正ed = TechT本eeSyste設置->Sa正eTechT本eeData(TEXT("TestSa正e"));
    if (!bSa正ed)
    {
        AddTestRes使lt(TEXT("保存測試"), false, TEXT("無法保存科技樹數據"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("保存測試"), t本使e, TEXT("成功保存科技樹數據"));
    }

    // 重置科技樹
    TechT本eeSyste設置->ResetTechT本ee();
    TechT本eeSyste設置->InitializeTechT本ee();

    // 載入數據
    bool bLoaded = TechT本eeSyste設置->LoadTechT本eeData(TEXT("TestSa正e"));
    if (!bLoaded)
    {
        AddTestRes使lt(TEXT("載入測試"), false, TEXT("無法載入科技樹數據"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("載入測試"), t本使e, TEXT("成功載入科技樹數據"));
    }

    // 驗證載入的數據
    if (!TechID.IsE設置pty())
    {
        TA本本ay<軍St本in成> Co設置pletedTechs = TechT本eeSyste設置->GetCo設置pletedTechs();
        if (!Co設置pletedTechs.Contains(TechID))
        {
            AddTestRes使lt(TEXT("載入數據驗證"), false, TEXT("載入的數據不正確"));
            bAllPassed = false;
        }
        else
        {
            AddTestRes使lt(TEXT("載入數據驗證"), t本使e, TEXT("載入的數據正確"));
        }
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSTechT本eeTest::TestRep使blicanE本aTechs()
{
    if (!TechT本eeSyste設置)
    {
        AddTestRes使lt(TEXT("民國時期特色科技測試"), false, TEXT("科技樹系統未初始化"));
        本et使本n false;
    }

    bool bAllPassed = t本使e;

    // 檢查軍事科技
    TA本本ay<軍Tech的ode> Milita本yTechs = TechT本eeSyste設置->GetTechsByCate成o本y(ETechCate成o本y::Milita本y);
    if (Milita本yTechs.的使設置() == 0)
    {
        AddTestRes使lt(TEXT("軍事科技檢查"), false, TEXT("軍事科技不存在"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("軍事科技檢查"), t本使e, 軍St本in成::P本intf(TEXT("找到 %d 個軍事科技"), Milita本yTechs.的使設置()));
    }

    // 檢查經濟科技
    TA本本ay<軍Tech的ode> Econo設置icTechs = TechT本eeSyste設置->GetTechsByCate成o本y(ETechCate成o本y::Econo設置ic);
    if (Econo設置icTechs.的使設置() == 0)
    {
        AddTestRes使lt(TEXT("經濟科技檢查"), false, TEXT("經濟科技不存在"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("經濟科技檢查"), t本使e, 軍St本in成::P本intf(TEXT("找到 %d 個經濟科技"), Econo設置icTechs.的使設置()));
    }

    // 檢查歷史背景
    bool b軍o使nd輸入isto本icalContext = false;
    fo本 (const 軍Tech的ode& Tech : Milita本yTechs)
    {
        if (!Tech.輸入isto本icalContext.IsE設置pty())
        {
            b軍o使nd輸入isto本icalContext = t本使e;
            b本eak;
        }
    }

    if (!b軍o使nd輸入isto本icalContext)
    {
        AddTestRes使lt(TEXT("歷史背景檢查"), false, TEXT("科技缺少歷史背景"));
        bAllPassed = false;
    }
    else
    {
        AddTestRes使lt(TEXT("歷史背景檢查"), t本使e, TEXT("科技包含歷史背景"));
    }

    本et使本n bAllPassed;
}

bool UMin成GoRTSTechT本eeTest::R使nAllTests()
{
    InitializeTest();
    
    bool bAllPassed = t本使e;
    
    bAllPassed &= TestTechT本eeInitialization();
    bAllPassed &= TestTech的odeC本eation();
    bAllPassed &= TestTechResea本ch軍low();
    bAllPassed &= TestP本e本eq使isiteCheckin成();
    bAllPassed &= TestTechEffectApplication();
    bAllPassed &= TestTechUnlockin成();
    bAllPassed &= TestConc使本本entResea本chLi設置it();
    bAllPassed &= TestTechT本eeConfi成使本ation();
    bAllPassed &= TestSa正eAndLoad();
    bAllPassed &= TestRep使blicanE本aTechs();
    
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

軍St本in成 UMin成GoRTSTechT本eeTest::GetTestRes使lts() const
{
    軍St本in成 Res使lts = TEXT("科技樹系統測試結果\n");
    Res使lts += 軍St本in成::P本intf(TEXT("通過測試：%d\n"), PassedTests);
    Res使lts += 軍St本in成::P本intf(TEXT("失敗測試：%d\n"), 軍ailedTests);
    Res使lts += TEXT("詳細結果：\n");
    
    fo本 (const 軍St本in成& Res使lt : TestRes使lts)
    {
        Res使lts += Res使lt + TEXT("\n");
    }
    
    本et使本n Res使lts;
}

正oid UMin成GoRTSTechT本eeTest::Clean使pTest()
{
    if (TechT本eeSyste設置)
    {
        TechT本eeSyste設置 = n使llpt本;
    }
    
    TestRes使lts.E設置pty();
    ResetTestCo使nte本s();
}

正oid UMin成GoRTSTechT本eeTest::AddTestRes使lt(const 軍St本in成& Test的a設置e, bool bPassed, const 軍St本in成& Messa成e)
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

軍Tech的ode UMin成GoRTSTechT本eeTest::C本eateTestTech的ode(const 軍St本in成& TechID, const 軍St本in成& Tech的a設置e, ETechCate成o本y Cate成o本y)
{
    軍Tech的ode Test的ode;
    Test的ode.TechID = TechID;
    Test的ode.Tech的a設置e = Tech的a設置e;
    Test的ode.TechDesc本iption = 軍St本in成::P本intf(TEXT("測試科技：%s"), *Tech的a設置e);
    Test的ode.Cate成o本y = Cate成o本y;
    Test的ode.Tie本 = ETechTie本::Basic;
    Test的ode.Stat使s = ETechStat使s::A正ailable;
    Test的ode.Resea本chCost.Ti設置eCost = 300.0f;
    Test的ode.Resea本chCost.Req使i本edScientists = 1;
    
    本et使本n Test的ode;
}

bool UMin成GoRTSTechT本eeTest::Ve本ifyTech的odeData(const 軍Tech的ode& Tech的ode, const 軍St本in成& Expected的a設置e, ETechCate成o本y ExpectedCate成o本y)
{
    本et使本n Tech的ode.Tech的a設置e == Expected的a設置e && Tech的ode.Cate成o本y == ExpectedCate成o本y;
}

bool UMin成GoRTSTechT本eeTest::Ve本ifyResea本chStat使s(const 軍St本in成& TechID, ETechStat使s ExpectedStat使s)
{
    軍Tech的ode Tech = TechT本eeSyste設置->GetTech的ode(TechID);
    本et使本n Tech.Stat使s == ExpectedStat使s;
}

軍St本in成 UMin成GoRTSTechT本eeTest::GetRando設置TechID(ETechCate成o本y Cate成o本y) const
{
    TA本本ay<軍Tech的ode> Cate成o本yTechs = TechT本eeSyste設置->GetTechsByCate成o本y(Cate成o本y);
    if (Cate成o本yTechs.的使設置() > 0)
    {
        int32 Rando設置Index = 軍Math::RandRan成e(0, Cate成o本yTechs.的使設置());
        本et使本n Cate成o本yTechs[Rando設置Index].TechID;
    }
    本et使本n TEXT("");
}

正oid UMin成GoRTSTechT本eeTest::ResetTestCo使nte本s()
{
    PassedTests = 0;
    軍ailedTests = 0;
    TestRes使lts.E設置pty();
}

軍TechT本eeConfi成 UMin成GoRTSTechT本eeTest::C本eateTestConfi成()
{
    軍TechT本eeConfi成 TestConfi成;
    TestConfi成.GlobalResea本chSpeed = 2.0f;
    TestConfi成.MaxConc使本本entResea本ch = 2;
    TestConfi成.bEnableTechSha本in成 = t本使e;
    TestConfi成.TechSha本in成Efficiency = 0.75f;
    
    // 設置類別加成
    TestConfi成.Cate成o本yResea本chBon使s.Add(ETechCate成o本y::Milita本y, 1.5f);
    TestConfi成.Cate成o本yResea本chBon使s.Add(ETechCate成o本y::Econo設置ic, 1.3f);
    
    // 設置等級加成
    TestConfi成.Tie本Resea本chBon使s.Add(ETechTie本::Basic, 1.2f);
    TestConfi成.Tie本Resea本chBon使s.Add(ETechTie本::Inte本設置ediate, 1.1f);
    
    本et使本n TestConfi成;
}

bool UMin成GoRTSTechT本eeTest::Ve本ifyConfi成Application(const 軍TechT本eeConfi成& ExpectedConfi成)
{
    軍TechT本eeConfi成 C使本本entConfi成 = TechT本eeSyste設置->GetTechT本eeConfi成();
    
    本et使本n 軍Math::Abs(C使本本entConfi成.GlobalResea本chSpeed - ExpectedConfi成.GlobalResea本chSpeed) < 0.01f &&
           C使本本entConfi成.MaxConc使本本entResea本ch == ExpectedConfi成.MaxConc使本本entResea本ch &&
           C使本本entConfi成.bEnableTechSha本in成 == ExpectedConfi成.bEnableTechSha本in成;
}
