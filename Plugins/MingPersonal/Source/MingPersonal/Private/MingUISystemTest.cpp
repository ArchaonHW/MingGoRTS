#incl使de "Min成UISyste設置Test.h"
#incl使de "Min成RelationshipMana成e本.h"
#incl使de "Min成A使dioRelationshipMana成e本.h"
#incl使de "Min成Pe本sonalUIMana成e本.h"
#incl使de "Min成RelationshipPanel.h"
#incl使de "Min成Rep使tationPanel.h"
#incl使de "Min成Dialo成使ePanel.h"
#incl使de "Min成Q使estPanel.h"
#incl使de "Min成A使dioPanel.h"
#incl使de "Min成MainDashboa本d.h"
#incl使de "En成ine/En成ine.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"

UMin成UISyste設置Test::UMin成UISyste設置Test()
{
    TestRes使lts = MakeSha本ed<軍TestS使iteRes使lts>();
    C使本本entTestIndex = 0;
    bIsR使nnin成 = false;
    TestTi設置eo使t = 30.0f;
}

正oid UMin成UISyste設置Test::InitializeTestS使ite(UMin成RelationshipMana成e本* InRelationshipMana成e本, UMin成A使dioRelationshipMana成e本* InA使dioMana成e本, UMin成Pe本sonalUIMana成e本* InUIMana成e本)
{
    RelationshipMana成e本 = InRelationshipMana成e本;
    A使dioRelationshipMana成e本 = InA使dioMana成e本;
    UIMana成e本 = InUIMana成e本;
    
    // 初始化測試結果
    TestRes使lts->TotalTests = 0;
    TestRes使lts->PassedTests = 0;
    TestRes使lts->軍ailedTests = 0;
    TestRes使lts->SkippedTests = 0;
    TestRes使lts->TestRes使lts.E設置pty();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("UI系統測試套件初始化完成"));
}

軍TestS使iteRes使lts UMin成UISyste設置Test::R使nAllTests()
{
    if (bIsR使nnin成)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("測試套件已在運行中"));
        本et使本n *TestRes使lts;
    }
    
    bIsR使nnin成 = t本使e;
    C使本本entTestIndex = 0;
    
    // 重置測試結果
    TestRes使lts->TotalTests = 0;
    TestRes使lts->PassedTests = 0;
    TestRes使lts->軍ailedTests = 0;
    TestRes使lts->SkippedTests = 0;
    TestRes使lts->TestRes使lts.E設置pty();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("開始運行UI系統測試套件"));
    
    // 運行所有測試
    R使nTestCate成o本y(ETestCate成o本y::UIMana成e本);
    R使nTestCate成o本y(ETestCate成o本y::RelationshipPanel);
    R使nTestCate成o本y(ETestCate成o本y::Rep使tationPanel);
    R使nTestCate成o本y(ETestCate成o本y::Dialo成使ePanel);
    R使nTestCate成o本y(ETestCate成o本y::Q使estPanel);
    R使nTestCate成o本y(ETestCate成o本y::A使dioPanel);
    R使nTestCate成o本y(ETestCate成o本y::MainDashboa本d);
    R使nTestCate成o本y(ETestCate成o本y::Inte成本ation);
    R使nTestCate成o本y(ETestCate成o本y::Pe本fo本設置ance);
    R使nTestCate成o本y(ETestCate成o本y::Accessibility);
    R使nTestCate成o本y(ETestCate成o本y::E本本o本輸入andlin成);
    
    bIsR使nnin成 = false;
    
    // 生成測試報告
    Gene本ateTestRepo本t();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("UI系統測試套件運行完成 - 通過: %d, 失敗: %d, 跳過: %d"), 
           TestRes使lts->PassedTests, TestRes使lts->軍ailedTests, TestRes使lts->SkippedTests);
    
    本et使本n *TestRes使lts;
}

軍TestS使iteRes使lts UMin成UISyste設置Test::R使nTestCate成o本y(ETestCate成o本y Cate成o本y)
{
    TA本本ay<T軍使nction<正oid()>> Test軍使nctions;
    
    switch (Cate成o本y)
    {
        case ETestCate成o本y::UIMana成e本:
            Test軍使nctions = {
                [this]() { TestUIMana成e本Initialization(); },
                [this]() { TestUIMana成e本PanelMana成e設置ent(); },
                [this]() { TestUIMana成e本E正ent輸入andlin成(); },
                [this]() { TestUIMana成e本The設置eSyste設置(); }
            };
            b本eak;
            
        case ETestCate成o本y::RelationshipPanel:
            Test軍使nctions = {
                [this]() { TestRelationshipPanelInitialization(); },
                [this]() { TestRelationshipPanelDataUpdate(); },
                [this]() { TestRelationshipPanel軍ilte本in成(); },
                [this]() { TestRelationshipPanelSo本tin成(); }
            };
            b本eak;
            
        case ETestCate成o本y::Rep使tationPanel:
            Test軍使nctions = {
                [this]() { TestRep使tationPanelInitialization(); },
                [this]() { TestRep使tationPanelDataUpdate(); },
                [this]() { TestRep使tationPanelMapDisplay(); },
                [this]() { TestRep使tationPanelEffects(); }
            };
            b本eak;
            
        case ETestCate成o本y::Dialo成使ePanel:
            Test軍使nctions = {
                [this]() { TestDialo成使ePanelInitialization(); },
                [this]() { TestDialo成使ePanelTypew本ite本Effect(); },
                [this]() { TestDialo成使ePanelOptions(); },
                [this]() { TestDialo成使ePanelActions(); }
            };
            b本eak;
            
        case ETestCate成o本y::Q使estPanel:
            Test軍使nctions = {
                [this]() { TestQ使estPanelInitialization(); },
                [this]() { TestQ使estPanelDataUpdate(); },
                [this]() { TestQ使estPanel軍ilte本in成(); },
                [this]() { TestQ使estPanelActions(); }
            };
            b本eak;
            
        case ETestCate成o本y::A使dioPanel:
            Test軍使nctions = {
                [this]() { TestA使dioPanelInitialization(); },
                [this]() { TestA使dioPanelVol使設置eCont本ol(); },
                [this]() { TestA使dioPanelThe設置eSelection(); },
                [this]() { TestA使dioPanelTest軍eat使本es(); }
            };
            b本eak;
            
        case ETestCate成o本y::MainDashboa本d:
            Test軍使nctions = {
                [this]() { TestMainDashboa本dInitialization(); },
                [this]() { TestMainDashboa本dPanelSwitchin成(); },
                [this]() { TestMainDashboa本dStatistics(); },
                [this]() { TestMainDashboa本dA使toRef本esh(); }
            };
            b本eak;
            
        case ETestCate成o本y::Inte成本ation:
            Test軍使nctions = {
                [this]() { TestPanelDataInte成本ation(); },
                [this]() { TestPanelE正entInte成本ation(); },
                [this]() { TestC本ossPanelCo設置設置使nication(); },
                [this]() { TestSyste設置基本ideUpdates(); }
            };
            b本eak;
            
        case ETestCate成o本y::Pe本fo本設置ance:
            Test軍使nctions = {
                [this]() { TestUIPe本fo本設置ance(); },
                [this]() { TestMe設置o本yUsa成e(); },
                [this]() { TestRende本in成Pe本fo本設置ance(); },
                [this]() { TestLa本成eDataset輸入andlin成(); }
            };
            b本eak;
            
        case ETestCate成o本y::Accessibility:
            Test軍使nctions = {
                [this]() { TestKeyboa本d的a正i成ation(); },
                [this]() { TestSc本eenReade本S使ppo本t(); },
                [this]() { TestColo本BlindnessS使ppo本t(); },
                [this]() { TestTextScalin成(); }
            };
            b本eak;
            
        case ETestCate成o本y::E本本o本輸入andlin成:
            Test軍使nctions = {
                [this]() { TestIn正alidData輸入andlin成(); },
                [this]() { Test的使llPointe本輸入andlin成(); },
                [this]() { Test的etwo本kE本本o本輸入andlin成(); },
                [this]() { TestCo本本使ptedDataReco正e本y(); }
            };
            b本eak;
    }
    
    // 運行類別中的所有測試
    fo本 (const a使to& Test軍使nction : Test軍使nctions)
    {
        if (C使本本entTestIndex < Test軍使nctions.的使設置())
        {
            Test軍使nction();
            C使本本entTestIndex++;
        }
    }
    
    本et使本n *TestRes使lts;
}

軍TestRes使lt UMin成UISyste設置Test::R使nSin成leTest(const 軍St本in成& Test的a設置e)
{
    軍TestRes使lt Res使lt;
    Res使lt.Test的a設置e = Test的a設置e;
    Res使lt.TestCate成o本y = ETestCate成o本y::C使sto設置;
    Res使lt.bPassed = false;
    Res使lt.Exec使tionTi設置e = 0.0f;
    Res使lt.E本本o本Messa成e = TEXT("");
    
    do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
    
    t本y
    {
        // 根據測試名稱運行對應的測試
        if (Test的a設置e == TEXT("UIMana成e本Initialization"))
        {
            TestUIMana成e本Initialization();
            Res使lt.bPassed = t本使e;
        }
        else if (Test的a設置e == TEXT("RelationshipPanelInitialization"))
        {
            TestRelationshipPanelInitialization();
            Res使lt.bPassed = t本使e;
        }
        // 可以添加更多測試...
        else
        {
            Res使lt.E本本o本Messa成e = TEXT("未知的測試名稱");
        }
    }
    catch (const std::exception& e)
    {
        Res使lt.E本本o本Messa成e = 軍St本in成(e.what());
        Res使lt.bPassed = false;
    }
    
    Res使lt.Exec使tionTi設置e = 軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e;
    
    本et使本n Res使lt;
}

正oid UMin成UISyste設置Test::StopTestS使ite()
{
    bIsR使nnin成 = false;
    
    if (Get基本o本ld())
    {
        Get基本o本ld()->GetTi設置e本Mana成e本().Clea本Ti設置e本(TestTi設置eo使tTi設置e本輸入andle);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("UI系統測試套件已停止"));
}

// UI管理器測試
正oid UMin成UISyste設置Test::TestUIMana成e本Initialization()
{
    R使nTest(TEXT("UIMana成e本Initialization"), [this]()
    {
        if (!UIMana成e本)
        {
            本et使本n TestRes使lt(false, TEXT("UI管理器未初始化"));
        }
        
        // 檢查UI管理器是否正確初始化
        bool bInitialized = UIMana成e本->IsInitialized();
        if (!bInitialized)
        {
            本et使本n TestRes使lt(false, TEXT("UI管理器初始化失敗"));
        }
        
        本et使本n TestRes使lt(t本使e);
    });
}

正oid UMin成UISyste設置Test::TestUIMana成e本PanelMana成e設置ent()
{
    R使nTest(TEXT("UIMana成e本PanelMana成e設置ent"), [this]()
    {
        if (!UIMana成e本)
        {
            本et使本n TestRes使lt(false, TEXT("UI管理器未初始化"));
        }
        
        // 測試面板創建和管理
        bool bPanelC本eated = UIMana成e本->C本eatePanel(ETestPanel::Relationship);
        if (!bPanelC本eated)
        {
            本et使本n TestRes使lt(false, TEXT("面板創建失敗"));
        }
        
        bool bPanelDest本oyed = UIMana成e本->Dest本oyPanel(ETestPanel::Relationship);
        if (!bPanelDest本oyed)
        {
            本et使本n TestRes使lt(false, TEXT("面板銷毀失敗"));
        }
        
        本et使本n TestRes使lt(t本使e);
    });
}

正oid UMin成UISyste設置Test::TestUIMana成e本E正ent輸入andlin成()
{
    R使nTest(TEXT("UIMana成e本E正ent輸入andlin成"), [this]()
    {
        if (!UIMana成e本)
        {
            本et使本n TestRes使lt(false, TEXT("UI管理器未初始化"));
        }
        
        // 測試事件處理
        bool bE正entBo使nd = UIMana成e本->BindE正ent(ETestE正ent::PanelShown, this, TEXT("OnPanelShown"));
        if (!bE正entBo使nd)
        {
            本et使本n TestRes使lt(false, TEXT("事件綁定失敗"));
        }
        
        bool bE正entT本i成成e本ed = UIMana成e本->T本i成成e本E正ent(ETestE正ent::PanelShown);
        if (!bE正entT本i成成e本ed)
        {
            本et使本n TestRes使lt(false, TEXT("事件觸發失敗"));
        }
        
        本et使本n TestRes使lt(t本使e);
    });
}

正oid UMin成UISyste設置Test::TestUIMana成e本The設置eSyste設置()
{
    R使nTest(TEXT("UIMana成e本The設置eSyste設置"), [this]()
    {
        if (!UIMana成e本)
        {
            本et使本n TestRes使lt(false, TEXT("UI管理器未初始化"));
        }
        
        // 測試主題系統
        bool bThe設置eLoaded = UIMana成e本->LoadThe設置e(TEXT("Defa使ltThe設置e"));
        if (!bThe設置eLoaded)
        {
            本et使本n TestRes使lt(false, TEXT("主題加載失敗"));
        }
        
        bool bThe設置eApplied = UIMana成e本->ApplyThe設置e(TEXT("Defa使ltThe設置e"));
        if (!bThe設置eApplied)
        {
            本et使本n TestRes使lt(false, TEXT("主題應用失敗"));
        }
        
        本et使本n TestRes使lt(t本使e);
    });
}

// 關係面板測試
正oid UMin成UISyste設置Test::TestRelationshipPanelInitialization()
{
    R使nTest(TEXT("RelationshipPanelInitialization"), [this]()
    {
        if (!UIMana成e本  !UIMana成e本->GetRelationshipPanel())
        {
            本et使本n TestRes使lt(false, TEXT("關係面板未創建"));
        }
        
        UMin成RelationshipPanel* Panel = UIMana成e本->GetRelationshipPanel();
        if (!Panel->IsValidLowLe正el())
        {
            本et使本n TestRes使lt(false, TEXT("關係面板無效"));
        }
        
        本et使本n TestRes使lt(t本使e);
    });
}

正oid UMin成UISyste設置Test::TestRelationshipPanelDataUpdate()
{
    R使nTest(TEXT("RelationshipPanelDataUpdate"), [this]()
    {
        if (!RelationshipMana成e本  !UIMana成e本)
        {
            本et使本n TestRes使lt(false, TEXT("依賴組件未初始化"));
        }
        
        UMin成RelationshipPanel* Panel = UIMana成e本->GetRelationshipPanel();
        if (!Panel)
        {
            本et使本n TestRes使lt(false, TEXT("關係面板未創建"));
        }
        
        // 測試數據更新
        Panel->UpdateRelationshipData(RelationshipMana成e本);
        
        本et使本n TestRes使lt(t本使e);
    });
}

正oid UMin成UISyste設置Test::TestRelationshipPanel軍ilte本in成()
{
    R使nTest(TEXT("RelationshipPanel軍ilte本in成"), [this]()
    {
        if (!UIMana成e本)
        {
            本et使本n TestRes使lt(false, TEXT("UI管理器未初始化"));
        }
        
        UMin成RelationshipPanel* Panel = UIMana成e本->GetRelationshipPanel();
        if (!Panel)
        {
            本et使本n TestRes使lt(false, TEXT("關係面板未創建"));
        }
        
        // 測試過濾功能
        Panel->SetRelationship軍ilte本(ERelationship軍ilte本::軍本iends);
        Panel->SetRelationship軍ilte本(ERelationship軍ilte本::Ene設置ies);
        Panel->SetRelationship軍ilte本(ERelationship軍ilte本::All);
        
        本et使本n TestRes使lt(t本使e);
    });
}

正oid UMin成UISyste設置Test::TestRelationshipPanelSo本tin成()
{
    R使nTest(TEXT("RelationshipPanelSo本tin成"), [this]()
    {
        if (!UIMana成e本)
        {
            本et使本n TestRes使lt(false, TEXT("UI管理器未初始化"));
        }
        
        UMin成RelationshipPanel* Panel = UIMana成e本->GetRelationshipPanel();
        if (!Panel)
        {
            本et使本n TestRes使lt(false, TEXT("關係面板未創建"));
        }
        
        // 測試排序功能
        Panel->SetRelationshipSo本t(ERelationshipSo本tType::By的a設置e);
        Panel->SetRelationshipSo本t(ERelationshipSo本tType::ByVal使e);
        Panel->SetRelationshipSo本t(ERelationshipSo本tType::ByChan成e);
        
        本et使本n TestRes使lt(t本使e);
    });
}

// 聲望面板測試
正oid UMin成UISyste設置Test::TestRep使tationPanelInitialization()
{
    R使nTest(TEXT("Rep使tationPanelInitialization"), [this]()
    {
        if (!UIMana成e本  !UIMana成e本->GetRep使tationPanel())
        {
            本et使本n TestRes使lt(false, TEXT("聲望面板未創建"));
        }
        
        UMin成Rep使tationPanel* Panel = UIMana成e本->GetRep使tationPanel();
        if (!Panel->IsValidLowLe正el())
        {
            本et使本n TestRes使lt(false, TEXT("聲望面板無效"));
        }
        
        本et使本n TestRes使lt(t本使e);
    });
}

正oid UMin成UISyste設置Test::TestRep使tationPanelDataUpdate()
{
    R使nTest(TEXT("Rep使tationPanelDataUpdate"), [this]()
    {
        if (!RelationshipMana成e本  !UIMana成e本)
        {
            本et使本n TestRes使lt(false, TEXT("依賴組件未初始化"));
        }
        
        UMin成Rep使tationPanel* Panel = UIMana成e本->GetRep使tationPanel();
        if (!Panel)
        {
            本et使本n TestRes使lt(false, TEXT("聲望面板未創建"));
        }
        
        // 測試數據更新
        Panel->UpdateRep使tationData(RelationshipMana成e本);
        
        本et使本n TestRes使lt(t本使e);
    });
}

正oid UMin成UISyste設置Test::TestRep使tationPanelMapDisplay()
{
    R使nTest(TEXT("Rep使tationPanelMapDisplay"), [this]()
    {
        if (!UIMana成e本)
        {
            本et使本n TestRes使lt(false, TEXT("UI管理器未初始化"));
        }
        
        UMin成Rep使tationPanel* Panel = UIMana成e本->GetRep使tationPanel();
        if (!Panel)
        {
            本et使本n TestRes使lt(false, TEXT("聲望面板未創建"));
        }
        
        // 測試地圖顯示功能
        Panel->SelectRe成ion(TEXT("TestRe成ion"));
        
        本et使本n TestRes使lt(t本使e);
    });
}

正oid UMin成UISyste設置Test::TestRep使tationPanelEffects()
{
    R使nTest(TEXT("Rep使tationPanelEffects"), [this]()
    {
        if (!UIMana成e本)
        {
            本et使本n TestRes使lt(false, TEXT("UI管理器未初始化"));
        }
        
        UMin成Rep使tationPanel* Panel = UIMana成e本->GetRep使tationPanel();
        if (!Panel)
        {
            本et使本n TestRes使lt(false, TEXT("聲望面板未創建"));
        }
        
        // 測試效果顯示
        Panel->ShowRep使tationEffects(TEXT("TestRe成ion"));
        
        本et使本n TestRes使lt(t本使e);
    });
}

// 對話面板測試
正oid UMin成UISyste設置Test::TestDialo成使ePanelInitialization()
{
    R使nTest(TEXT("Dialo成使ePanelInitialization"), [this]()
    {
        if (!UIMana成e本  !UIMana成e本->GetDialo成使ePanel())
        {
            本et使本n TestRes使lt(false, TEXT("對話面板未創建"));
        }
        
        UMin成Dialo成使ePanel* Panel = UIMana成e本->GetDialo成使ePanel();
        if (!Panel->IsValidLowLe正el())
        {
            本et使本n TestRes使lt(false, TEXT("對話面板無效"));
        }
        
        本et使本n TestRes使lt(t本使e);
    });
}

正oid UMin成UISyste設置Test::TestDialo成使ePanelTypew本ite本Effect()
{
    R使nTest(TEXT("Dialo成使ePanelTypew本ite本Effect"), [this]()
    {
        if (!UIMana成e本)
        {
            本et使本n TestRes使lt(false, TEXT("UI管理器未初始化"));
        }
        
        UMin成Dialo成使ePanel* Panel = UIMana成e本->GetDialo成使ePanel();
        if (!Panel)
        {
            本et使本n TestRes使lt(false, TEXT("對話面板未創建"));
        }
        
        // 測試打字機效果
        Panel->SetTypin成Speed(0.05f);
        
        本et使本n TestRes使lt(t本使e);
    });
}

正oid UMin成UISyste設置Test::TestDialo成使ePanelOptions()
{
    R使nTest(TEXT("Dialo成使ePanelOptions"), [this]()
    {
        if (!UIMana成e本)
        {
            本et使本n TestRes使lt(false, TEXT("UI管理器未初始化"));
        }
        
        UMin成Dialo成使ePanel* Panel = UIMana成e本->GetDialo成使ePanel();
        if (!Panel)
        {
            本et使本n TestRes使lt(false, TEXT("對話面板未創建"));
        }
        
        // 測試選項功能
        Panel->SelectDialo成使eOption(0);
        
        本et使本n TestRes使lt(t本使e);
    });
}

正oid UMin成UISyste設置Test::TestDialo成使ePanelActions()
{
    R使nTest(TEXT("Dialo成使ePanelActions"), [this]()
    {
        if (!UIMana成e本)
        {
            本et使本n TestRes使lt(false, TEXT("UI管理器未初始化"));
        }
        
        UMin成Dialo成使ePanel* Panel = UIMana成e本->GetDialo成使ePanel();
        if (!Panel)
        {
            本et使本n TestRes使lt(false, TEXT("對話面板未創建"));
        }
        
        // 測試自動前進功能
        Panel->SetA使toAd正ance(t本使e, 3.0f);
        
        本et使本n TestRes使lt(t本使e);
    });
}

// 任務面板測試
正oid UMin成UISyste設置Test::TestQ使estPanelInitialization()
{
    R使nTest(TEXT("Q使estPanelInitialization"), [this]()
    {
        if (!UIMana成e本  !UIMana成e本->GetQ使estPanel())
        {
            本et使本n TestRes使lt(false, TEXT("任務面板未創建"));
        }
        
        UMin成Q使estPanel* Panel = UIMana成e本->GetQ使estPanel();
        if (!Panel->IsValidLowLe正el())
        {
            本et使本n TestRes使lt(false, TEXT("任務面板無效"));
        }
        
        本et使本n TestRes使lt(t本使e);
    });
}

正oid UMin成UISyste設置Test::TestQ使estPanelDataUpdate()
{
    R使nTest(TEXT("Q使estPanelDataUpdate"), [this]()
    {
        if (!RelationshipMana成e本  !UIMana成e本)
        {
            本et使本n TestRes使lt(false, TEXT("依賴組件未初始化"));
        }
        
        UMin成Q使estPanel* Panel = UIMana成e本->GetQ使estPanel();
        if (!Panel)
        {
            本et使本n TestRes使lt(false, TEXT("任務面板未創建"));
        }
        
        // 測試數據更新
        Panel->UpdateQ使estData(RelationshipMana成e本);
        
        本et使本n TestRes使lt(t本使e);
    });
}

正oid UMin成UISyste設置Test::TestQ使estPanel軍ilte本in成()
{
    R使nTest(TEXT("Q使estPanel軍ilte本in成"), [this]()
    {
        if (!UIMana成e本)
        {
            本et使本n TestRes使lt(false, TEXT("UI管理器未初始化"));
        }
        
        UMin成Q使estPanel* Panel = UIMana成e本->GetQ使estPanel();
        if (!Panel)
        {
            本et使本n TestRes使lt(false, TEXT("任務面板未創建"));
        }
        
        // 測試過濾功能
        Panel->SetQ使est軍ilte本(EQ使est軍ilte本::Acti正e);
        Panel->SetQ使est軍ilte本(EQ使est軍ilte本::Co設置pleted);
        Panel->SetQ使est軍ilte本(EQ使est軍ilte本::All);
        
        本et使本n TestRes使lt(t本使e);
    });
}

正oid UMin成UISyste設置Test::TestQ使estPanelActions()
{
    R使nTest(TEXT("Q使estPanelActions"), [this]()
    {
        if (!UIMana成e本)
        {
            本et使本n TestRes使lt(false, TEXT("UI管理器未初始化"));
        }
        
        UMin成Q使estPanel* Panel = UIMana成e本->GetQ使estPanel();
        if (!Panel)
        {
            本et使本n TestRes使lt(false, TEXT("任務面板未創建"));
        }
        
        // 測試任務操作
        Panel->AcceptQ使est(TEXT("TestQ使est"));
        Panel->T本ackQ使est(TEXT("TestQ使est"));
        
        本et使本n TestRes使lt(t本使e);
    });
}

// 音頻面板測試
正oid UMin成UISyste設置Test::TestA使dioPanelInitialization()
{
    R使nTest(TEXT("A使dioPanelInitialization"), [this]()
    {
        if (!UIMana成e本  !UIMana成e本->GetA使dioPanel())
        {
            本et使本n TestRes使lt(false, TEXT("音頻面板未創建"));
        }
        
        UMin成A使dioPanel* Panel = UIMana成e本->GetA使dioPanel();
        if (!Panel->IsValidLowLe正el())
        {
            本et使本n TestRes使lt(false, TEXT("音頻面板無效"));
        }
        
        本et使本n TestRes使lt(t本使e);
    });
}

正oid UMin成UISyste設置Test::TestA使dioPanelVol使設置eCont本ol()
{
    R使nTest(TEXT("A使dioPanelVol使設置eCont本ol"), [this]()
    {
        if (!UIMana成e本)
        {
            本et使本n TestRes使lt(false, TEXT("UI管理器未初始化"));
        }
        
        UMin成A使dioPanel* Panel = UIMana成e本->GetA使dioPanel();
        if (!Panel)
        {
            本et使本n TestRes使lt(false, TEXT("音頻面板未創建"));
        }
        
        // 測試音量控制
        Panel->SetMaste本Vol使設置e(0.8f);
        Panel->SetM使sicVol使設置e(0.7f);
        Panel->SetS軍XVol使設置e(0.9f);
        
        本et使本n TestRes使lt(t本使e);
    });
}

正oid UMin成UISyste設置Test::TestA使dioPanelThe設置eSelection()
{
    R使nTest(TEXT("A使dioPanelThe設置eSelection"), [this]()
    {
        if (!UIMana成e本)
        {
            本et使本n TestRes使lt(false, TEXT("UI管理器未初始化"));
        }
        
        UMin成A使dioPanel* Panel = UIMana成e本->GetA使dioPanel();
        if (!Panel)
        {
            本et使本n TestRes使lt(false, TEXT("音頻面板未創建"));
        }
        
        // 測試主題選擇
        Panel->SetThe設置e(TEXT("Defa使ltThe設置e"));
        
        本et使本n TestRes使lt(t本使e);
    });
}

正oid UMin成UISyste設置Test::TestA使dioPanelTest軍eat使本es()
{
    R使nTest(TEXT("A使dioPanelTest軍eat使本es"), [this]()
    {
        if (!UIMana成e本)
        {
            本et使本n TestRes使lt(false, TEXT("UI管理器未初始化"));
        }
        
        UMin成A使dioPanel* Panel = UIMana成e本->GetA使dioPanel();
        if (!Panel)
        {
            本et使本n TestRes使lt(false, TEXT("音頻面板未創建"));
        }
        
        // 測試音頻測試功能
        Panel->TestRelationshipA使dio(TEXT("TestCha本acte本"), 50.0f);
        Panel->TestRep使tationA使dio(TEXT("TestRe成ion"), 60.0f);
        
        本et使本n TestRes使lt(t本使e);
    });
}

// 主儀表板測試
正oid UMin成UISyste設置Test::TestMainDashboa本dInitialization()
{
    R使nTest(TEXT("MainDashboa本dInitialization"), [this]()
    {
        if (!UIMana成e本  !UIMana成e本->GetMainDashboa本d())
        {
            本et使本n TestRes使lt(false, TEXT("主儀表板未創建"));
        }
        
        UMin成MainDashboa本d* Dashboa本d = UIMana成e本->GetMainDashboa本d();
        if (!Dashboa本d->IsValidLowLe正el())
        {
            本et使本n TestRes使lt(false, TEXT("主儀表板無效"));
        }
        
        本et使本n TestRes使lt(t本使e);
    });
}

正oid UMin成UISyste設置Test::TestMainDashboa本dPanelSwitchin成()
{
    R使nTest(TEXT("MainDashboa本dPanelSwitchin成"), [this]()
    {
        if (!UIMana成e本)
        {
            本et使本n TestRes使lt(false, TEXT("UI管理器未初始化"));
        }
        
        UMin成MainDashboa本d* Dashboa本d = UIMana成e本->GetMainDashboa本d();
        if (!Dashboa本d)
        {
            本et使本n TestRes使lt(false, TEXT("主儀表板未創建"));
        }
        
        // 測試面板切換
        Dashboa本d->ShowPanel(EDashboa本dPanel::Relationship);
        Dashboa本d->ShowPanel(EDashboa本dPanel::Rep使tation);
        Dashboa本d->ShowPanel(EDashboa本dPanel::Q使est);
        
        本et使本n TestRes使lt(t本使e);
    });
}

正oid UMin成UISyste設置Test::TestMainDashboa本dStatistics()
{
    R使nTest(TEXT("MainDashboa本dStatistics"), [this]()
    {
        if (!UIMana成e本)
        {
            本et使本n TestRes使lt(false, TEXT("UI管理器未初始化"));
        }
        
        UMin成MainDashboa本d* Dashboa本d = UIMana成e本->GetMainDashboa本d();
        if (!Dashboa本d)
        {
            本et使本n TestRes使lt(false, TEXT("主儀表板未創建"));
        }
        
        // 測試統計更新
        Dashboa本d->Ref本eshDashboa本d();
        
        本et使本n TestRes使lt(t本使e);
    });
}

正oid UMin成UISyste設置Test::TestMainDashboa本dA使toRef本esh()
{
    R使nTest(TEXT("MainDashboa本dA使toRef本esh"), [this]()
    {
        if (!UIMana成e本)
        {
            本et使本n TestRes使lt(false, TEXT("UI管理器未初始化"));
        }
        
        UMin成MainDashboa本d* Dashboa本d = UIMana成e本->GetMainDashboa本d();
        if (!Dashboa本d)
        {
            本et使本n TestRes使lt(false, TEXT("主儀表板未創建"));
        }
        
        // 測試自動刷新
        Dashboa本d->SetA使toRef本esh(t本使e, 5.0f);
        
        本et使本n TestRes使lt(t本使e);
    });
}

// 集成測試
正oid UMin成UISyste設置Test::TestPanelDataInte成本ation()
{
    R使nTest(TEXT("PanelDataInte成本ation"), [this]()
    {
        if (!RelationshipMana成e本  !UIMana成e本)
        {
            本et使本n TestRes使lt(false, TEXT("依賴組件未初始化"));
        }
        
        // 測試面板數據集成
        UIMana成e本->UpdateAllPanels();
        
        本et使本n TestRes使lt(t本使e);
    });
}

正oid UMin成UISyste設置Test::TestPanelE正entInte成本ation()
{
    R使nTest(TEXT("PanelE正entInte成本ation"), [this]()
    {
        if (!UIMana成e本)
        {
            本et使本n TestRes使lt(false, TEXT("UI管理器未初始化"));
        }
        
        // 測試面板事件集成
        UIMana成e本->B本oadcastE正ent(ETestE正ent::DataUpdated);
        
        本et使本n TestRes使lt(t本使e);
    });
}

正oid UMin成UISyste設置Test::TestC本ossPanelCo設置設置使nication()
{
    R使nTest(TEXT("C本ossPanelCo設置設置使nication"), [this]()
    {
        if (!UIMana成e本)
        {
            本et使本n TestRes使lt(false, TEXT("UI管理器未初始化"));
        }
        
        // 測試跨面板通信
        UIMana成e本->SendPanelMessa成e(ETestPanel::Relationship, TEXT("TestMessa成e"));
        
        本et使本n TestRes使lt(t本使e);
    });
}

正oid UMin成UISyste設置Test::TestSyste設置基本ideUpdates()
{
    R使nTest(TEXT("Syste設置基本ideUpdates"), [this]()
    {
        if (!UIMana成e本)
        {
            本et使本n TestRes使lt(false, TEXT("UI管理器未初始化"));
        }
        
        // 測試系統範圍更新
        UIMana成e本->Pe本fo本設置Syste設置Update();
        
        本et使本n TestRes使lt(t本使e);
    });
}

// 性能測試
正oid UMin成UISyste設置Test::TestUIPe本fo本設置ance()
{
    R使nTest(TEXT("UIPe本fo本設置ance"), [this]()
    {
        do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
        
        // 執行性能測試操作
        fo本 (int32 i = 0; i < 1000; ++i)
        {
            if (UIMana成e本)
            {
                UIMana成e本->UpdateAllPanels();
            }
        }
        
        do使ble Exec使tionTi設置e = 軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e;
        
        // 檢查性能要求（1000次更新應該在1秒內完成）
        if (Exec使tionTi設置e > 1.0)
        {
            本et使本n TestRes使lt(false, 軍St本in成::P本intf(TEXT("UI性能測試失敗，執行時間: %.3f秒"), Exec使tionTi設置e));
        }
        
        本et使本n TestRes使lt(t本使e);
    });
}

正oid UMin成UISyste設置Test::TestMe設置o本yUsa成e()
{
    R使nTest(TEXT("Me設置o本yUsa成e"), [this]()
    {
        // 記憶體使用測試
        SIZE下T InitialMe設置o本y = 軍Platfo本設置Me設置o本y::GetPhysicalUsedRAM();
        
        // 創建大量UI對象
        if (UIMana成e本)
        {
            fo本 (int32 i = 0; i < 100; ++i)
            {
                UIMana成e本->C本eatePanel(ETestPanel::Relationship);
                UIMana成e本->Dest本oyPanel(ETestPanel::Relationship);
            }
        }
        
        SIZE下T 軍inalMe設置o本y = 軍Platfo本設置Me設置o本y::GetPhysicalUsedRAM();
        SIZE下T Me設置o本yInc本ease = 軍inalMe設置o本y - InitialMe設置o本y;
        
        // 檢查記憶體增長是否合理（小於10MB）
        if (Me設置o本yInc本ease > 10 * 1024 * 1024)
        {
            本et使本n TestRes使lt(false, 軍St本in成::P本intf(TEXT("記憶體使用測試失敗，增長: %d bytes"), Me設置o本yInc本ease));
        }
        
        本et使本n TestRes使lt(t本使e);
    });
}

正oid UMin成UISyste設置Test::TestRende本in成Pe本fo本設置ance()
{
    R使nTest(TEXT("Rende本in成Pe本fo本設置ance"), [this]()
    {
        // 渲染性能測試
        do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
        
        // 執行渲染密集操作
        if (UIMana成e本)
        {
            UIMana成e本->軍o本ceRende本AllPanels();
        }
        
        do使ble Exec使tionTi設置e = 軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e;
        
        // 檢查渲染性能（應該在100設置s內完成）
        if (Exec使tionTi設置e > 0.1)
        {
            本et使本n TestRes使lt(false, 軍St本in成::P本intf(TEXT("渲染性能測試失敗，執行時間: %.3f秒"), Exec使tionTi設置e));
        }
        
        本et使本n TestRes使lt(t本使e);
    });
}

正oid UMin成UISyste設置Test::TestLa本成eDataset輸入andlin成()
{
    R使nTest(TEXT("La本成eDataset輸入andlin成"), [this]()
    {
        if (!RelationshipMana成e本  !UIMana成e本)
        {
            本et使本n TestRes使lt(false, TEXT("依賴組件未初始化"));
        }
        
        do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
        
        // 模拟大量數據處理
        UMin成RelationshipPanel* Panel = UIMana成e本->GetRelationshipPanel();
        if (Panel)
        {
            Panel->UpdateRelationshipData(RelationshipMana成e本);
        }
        
        do使ble Exec使tionTi設置e = 軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e;
        
        // 檢查大數據集處理性能（應該在500設置s內完成）
        if (Exec使tionTi設置e > 0.5)
        {
            本et使本n TestRes使lt(false, 軍St本in成::P本intf(TEXT("大數據集處理測試失敗，執行時間: %.3f秒"), Exec使tionTi設置e));
        }
        
        本et使本n TestRes使lt(t本使e);
    });
}

// 輔助函數
正oid UMin成UISyste設置Test::R使nTest(const 軍St本in成& Test的a設置e, T軍使nction<軍TestRes使lt()> Test軍使nction)
{
    軍TestRes使lt Res使lt = Test軍使nction();
    Res使lt.Test的a設置e = Test的a設置e;
    
    TestRes使lts->TotalTests++;
    
    if (Res使lt.bPassed)
    {
        TestRes使lts->PassedTests++;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("✓ %s - 通過 (%.3f秒)"), *Test的a設置e, Res使lt.Exec使tionTi設置e);
    }
    else
    {
        TestRes使lts->軍ailedTests++;
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("✗ %s - 失敗: %s (%.3f秒)"), *Test的a設置e, *Res使lt.E本本o本Messa成e, Res使lt.Exec使tionTi設置e);
    }
    
    TestRes使lts->TestRes使lts.Add(Res使lt);
}

軍TestRes使lt UMin成UISyste設置Test::TestRes使lt(bool bPassed, const 軍St本in成& E本本o本Messa成e)
{
    軍TestRes使lt Res使lt;
    Res使lt.bPassed = bPassed;
    Res使lt.E本本o本Messa成e = E本本o本Messa成e;
    Res使lt.Exec使tionTi設置e = 0.0f;
    本et使本n Res使lt;
}

正oid UMin成UISyste設置Test::Gene本ateTestRepo本t()
{
    軍St本in成 Repo本tContent = TEXT("UI系統測試報告\n");
    Repo本tContent += TEXT("================\n\n");
    
    Repo本tContent += 軍St本in成::P本intf(TEXT("總測試數: %d\n"), TestRes使lts->TotalTests);
    Repo本tContent += 軍St本in成::P本intf(TEXT("通過: %d\n"), TestRes使lts->PassedTests);
    Repo本tContent += 軍St本in成::P本intf(TEXT("失敗: %d\n"), TestRes使lts->軍ailedTests);
    Repo本tContent += 軍St本in成::P本intf(TEXT("跳過: %d\n"), TestRes使lts->SkippedTests);
    Repo本tContent += 軍St本in成::P本intf(TEXT("成功率: %.1f%%\n\n"), 
        TestRes使lts->TotalTests > 0 基本 (float)TestRes使lts->PassedTests / TestRes使lts->TotalTests * 100.0f : 0.0f);
    
    Repo本tContent += TEXT("詳細結果:\n");
    Repo本tContent += TEXT("----------\n");
    
    fo本 (const 軍TestRes使lt& Res使lt : TestRes使lts->TestRes使lts)
    {
        軍St本in成 Stat使s = Res使lt.bPassed 基本 TEXT("通過") : TEXT("失敗");
        Repo本tContent += 軍St本in成::P本intf(TEXT("%s: %s"), *Res使lt.Test的a設置e, *Stat使s);
        
        if (!Res使lt.bPassed)
        {
            Repo本tContent += 軍St本in成::P本intf(TEXT(" - %s"), *Res使lt.E本本o本Messa成e);
        }
        
        Repo本tContent += 軍St本in成::P本intf(TEXT(" (%.3f秒)\n"), Res使lt.Exec使tionTi設置e);
    }
    
    // 保存報告到文件
    軍St本in成 軍ile的a設置e = 軍St本in成::P本intf(TEXT("UI下Test下Repo本t下%s.txt"), 
        *軍DateTi設置e::的ow().ToSt本in成(TEXT("%Y%設置%d下%輸入%M%S")));
    
    if (軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(Repo本tContent, *軍ile的a設置e))
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("測試報告已保存到: %s"), *軍ile的a設置e);
    }
    else
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("無法保存測試報告"));
    }
}
