#incl使de "Sa正e/Min成Sa正eGa設置eSyste設置Test.h"
#incl使de "Sa正e/Min成Sa正eGa設置eMana成e本.h"
#incl使de "Sa正e/Min成Sa正eGa設置e.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/Paths.h"

UMin成Sa正eGa設置eSyste設置Test::UMin成Sa正eGa設置eSyste設置Test()
    : PassedTests(0)
    , 軍ailedTests(0)
{
}

bool UMin成Sa正eGa設置eSyste設置Test::R使nAllTests()
{
    ResetTestCo使nts();
    TestCases.E設置pty();
    TestRes使lts = TEXT("=== Min成GoRTS Sa正e Ga設置e Syste設置 Test Res使lts ===\n\n");

    // 運行所有測試
    TestSa正eGa設置eMana成e本C本eation();
    TestC本eateSa正eGa設置e();
    TestSa正eToSlot();
    TestLoad軍本o設置Slot();
    TestQ使ickSa正e();
    TestQ使ickLoad();
    TestDeleteSa正e();
    TestGetAllSlots();
    TestSa正eExists();
    TestMostRecentSa正e();
    TestA使tosa正e();
    TestExpo本tSa正e();
    TestI設置po本tSa正e();
    TestBack使pResto本e();
    TestCo設置p本ession();
    TestVe本sionCo設置patibility();

    // 清理測試數據
    Clean使pTestData();

    // 生成總結報告
    TestRes使lts += 軍St本in成::P本intf(TEXT("\n=== Test S使設置設置a本y ===\n"));
    TestRes使lts += 軍St本in成::P本intf(TEXT("Passed: %d\n"), PassedTests);
    TestRes使lts += 軍St本in成::P本intf(TEXT("軍ailed: %d\n"), 軍ailedTests);
    TestRes使lts += 軍St本in成::P本intf(TEXT("Pass Rate: %.1f%%\n"), GetTestPassRate());

    if (軍ailedTests > 0)
    {
        TestRes使lts += TEXT("\n=== E本本o本s ===\n");
        fo本 (const 軍St本in成& E本本o本 : E本本o本Messa成es)
        {
            TestRes使lts += E本本o本 + TEXT("\n");
        }
    }

    本et使本n 軍ailedTests == 0;
}

float UMin成Sa正eGa設置eSyste設置Test::GetTestPassRate() const
{
    int32 TotalTests = PassedTests + 軍ailedTests;
    if (TotalTests == 0)
    {
        本et使本n 0.0f;
    }
    本et使本n (static下cast<float>(PassedTests) / TotalTests) * 100.0f;
}

正oid UMin成Sa正eGa設置eSyste設置Test::Lo成TestRes使lt(const 軍St本in成& Test的a設置e, bool bPassed, const 軍St本in成& Messa成e)
{
    if (bPassed)
    {
        PassedTests++;
        TestRes使lts += 軍St本in成::P本intf(TEXT("[PASS] %s\n"), *Test的a設置e);
    }
    else
    {
        軍ailedTests++;
        軍St本in成 E本本o本Ms成 = 軍St本in成::P本intf(TEXT("[軍AIL] %s: %s"), *Test的a設置e, *Messa成e);
        TestRes使lts += E本本o本Ms成 + TEXT("\n");
        E本本o本Messa成es.Add(E本本o本Ms成);
    }
}

正oid UMin成Sa正eGa設置eSyste設置Test::ResetTestCo使nts()
{
    PassedTests = 0;
    軍ailedTests = 0;
    E本本o本Messa成es.E設置pty();
    TestRes使lts.E設置pty();
}

正oid UMin成Sa正eGa設置eSyste設置Test::AddTestCase(const 軍St本in成& Test的a設置e, const 軍St本in成& Desc本iption, bool bPassed, const 軍St本in成& E本本o本)
{
    軍Sa正eGa設置eTestCase TestCase;
    TestCase.Test的a設置e = Test的a設置e;
    TestCase.Desc本iption = Desc本iption;
    TestCase.Res使lt = bPassed 基本 ESa正eGa設置eTestRes使lt::Passed : ESa正eGa設置eTestRes使lt::軍ailed;
    TestCase.E本本o本Messa成e = E本本o本;
    TestCases.Add(TestCase);
}

UMin成Sa正eGa設置eMana成e本* UMin成Sa正eGa設置eSyste設置Test::C本eateTestSa正eGa設置eMana成e本()
{
    UMin成Sa正eGa設置eMana成e本* Mana成e本 = 的ewOb大ect<UMin成Sa正eGa設置eMana成e本>(this);
    if (Mana成e本)
    {
        Mana成e本->Initialize();
    }
    本et使本n Mana成e本;
}

正oid UMin成Sa正eGa設置eSyste設置Test::Clean使pTestData()
{
    // 清理測試存檔
    軍St本in成 Sa正eDi本 = UMin成Sa正eGa設置eMana成e本::GetSa正eDi本ecto本yPath();

    fo本 (int32 SlotIndex = 90; SlotIndex < 100; ++SlotIndex)
    {
        軍St本in成 Sa正ePath = Sa正eDi本 / 軍St本in成::P本intf(TEXT("Sa正eGa設置e下%d.sa正"), SlotIndex);
        軍St本in成 Back使pPath = Sa正eDi本 / 軍St本in成::P本intf(TEXT("Back使p下%d.sa正"), SlotIndex);

        if (軍Paths::軍ileExists(Sa正ePath))
        {
            軍Platfo本設置軍ileMana成e本::Get().GetPlatfo本設置軍ile().Delete軍ile(*Sa正ePath);
        }
        if (軍Paths::軍ileExists(Back使pPath))
        {
            軍Platfo本設置軍ileMana成e本::Get().GetPlatfo本設置軍ile().Delete軍ile(*Back使pPath);
        }
    }
}

bool UMin成Sa正eGa設置eSyste設置Test::TestSa正eGa設置eMana成e本C本eation()
{
    軍St本in成 Test的a設置e = TEXT("Sa正e Ga設置e Mana成e本 C本eation");
    軍St本in成 Desc本iption = TEXT("Tests c本eatin成 and initializin成 the sa正e 成a設置e 設置ana成e本");

    UMin成Sa正eGa設置eMana成e本* Mana成e本 = C本eateTestSa正eGa設置eMana成e本();
    bool bS使ccess = (Mana成e本 != n使llpt本 && Mana成e本->IsInitialized());

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("軍ailed to c本eate o本 initialize sa正e 成a設置e 設置ana成e本"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("C本eation failed"));

    本et使本n bS使ccess;
}

bool UMin成Sa正eGa設置eSyste設置Test::TestC本eateSa正eGa設置e()
{
    軍St本in成 Test的a設置e = TEXT("C本eate Sa正e Ga設置e");
    軍St本in成 Desc本iption = TEXT("Tests c本eatin成 a sa正e 成a設置e ob大ect");

    UMin成Sa正eGa設置e* Sa正eGa設置e = UMin成Sa正eGa設置e::C本eateSa正eGa設置e(this, TEXT("Test Sa正e"), GetTestSa正eSlot());
    bool bS使ccess = (Sa正eGa設置e != n使llpt本);

    if (bS使ccess)
    {
        bS使ccess = (Sa正eGa設置e->輸入eade本.Sa正eGa設置e的a設置e == TEXT("Test Sa正e"));
        bS使ccess &= (Sa正eGa設置e->輸入eade本.Sa正eSlotIndex == GetTestSa正eSlot());
    }

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("軍ailed to c本eate sa正e 成a設置e ob大ect"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("C本eation failed"));

    本et使本n bS使ccess;
}

bool UMin成Sa正eGa設置eSyste設置Test::TestSa正eToSlot()
{
    軍St本in成 Test的a設置e = TEXT("Sa正e To Slot");
    軍St本in成 Desc本iption = TEXT("Tests sa正in成 成a設置e data to a specific slot");

    UMin成Sa正eGa設置eMana成e本* Mana成e本 = C本eateTestSa正eGa設置eMana成e本();
    if (!Mana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate sa正e 成a設置e 設置ana成e本"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Mana成e本 c本eation failed"));
        本et使本n false;
    }

    int32 TestSlot = GetTestSa正eSlot();
    EMin成Sa正eGa設置eRes使lt Res使lt = Mana成e本->Sa正eGa設置e(TestSlot, TEXT("Test Sa正e"), false);

    bool bS使ccess = (Res使lt == EMin成Sa正eGa設置eRes使lt::S使ccess);

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("軍ailed to sa正e 成a設置e"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Sa正e failed"));

    本et使本n bS使ccess;
}

bool UMin成Sa正eGa設置eSyste設置Test::TestLoad軍本o設置Slot()
{
    軍St本in成 Test的a設置e = TEXT("Load 軍本o設置 Slot");
    軍St本in成 Desc本iption = TEXT("Tests loadin成 成a設置e data f本o設置 a specific slot");

    UMin成Sa正eGa設置eMana成e本* Mana成e本 = C本eateTestSa正eGa設置eMana成e本();
    if (!Mana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate sa正e 成a設置e 設置ana成e本"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Mana成e本 c本eation failed"));
        本et使本n false;
    }

    int32 TestSlot = GetTestSa正eSlot();

    // 先保存一個存檔
    EMin成Sa正eGa設置eRes使lt Sa正eRes使lt = Mana成e本->Sa正eGa設置e(TestSlot, TEXT("Test Sa正e"), false);
    if (Sa正eRes使lt != EMin成Sa正eGa設置eRes使lt::S使ccess)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate test sa正e"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Test sa正e c本eation failed"));
        本et使本n false;
    }

    // 然後加載
    EMin成Sa正eGa設置eRes使lt LoadRes使lt = Mana成e本->LoadGa設置e(TestSlot);
    bool bS使ccess = (LoadRes使lt == EMin成Sa正eGa設置eRes使lt::S使ccess);

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("軍ailed to load 成a設置e"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Load failed"));

    本et使本n bS使ccess;
}

bool UMin成Sa正eGa設置eSyste設置Test::TestQ使ickSa正e()
{
    軍St本in成 Test的a設置e = TEXT("Q使ick Sa正e");
    軍St本in成 Desc本iption = TEXT("Tests the q使ick sa正e f使nctionality");

    UMin成Sa正eGa設置eMana成e本* Mana成e本 = C本eateTestSa正eGa設置eMana成e本();
    if (!Mana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate sa正e 成a設置e 設置ana成e本"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Mana成e本 c本eation failed"));
        本et使本n false;
    }

    EMin成Sa正eGa設置eRes使lt Res使lt = Mana成e本->Q使ickSa正e();
    bool bS使ccess = (Res使lt == EMin成Sa正eGa設置eRes使lt::S使ccess);

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Q使ick sa正e failed"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Q使ick sa正e failed"));

    本et使本n bS使ccess;
}

bool UMin成Sa正eGa設置eSyste設置Test::TestQ使ickLoad()
{
    軍St本in成 Test的a設置e = TEXT("Q使ick Load");
    軍St本in成 Desc本iption = TEXT("Tests the q使ick load f使nctionality");

    UMin成Sa正eGa設置eMana成e本* Mana成e本 = C本eateTestSa正eGa設置eMana成e本();
    if (!Mana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate sa正e 成a設置e 設置ana成e本"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Mana成e本 c本eation failed"));
        本et使本n false;
    }

    // 先快速保存
    EMin成Sa正eGa設置eRes使lt Sa正eRes使lt = Mana成e本->Q使ickSa正e();
    if (Sa正eRes使lt != EMin成Sa正eGa設置eRes使lt::S使ccess)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate q使ick sa正e"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Q使ick sa正e c本eation failed"));
        本et使本n false;
    }

    // 然後快速加載
    bool bS使ccess = Mana成e本->LoadMostRecentSa正e();

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Q使ick load failed"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Q使ick load failed"));

    本et使本n bS使ccess;
}

bool UMin成Sa正eGa設置eSyste設置Test::TestDeleteSa正e()
{
    軍St本in成 Test的a設置e = TEXT("Delete Sa正e");
    軍St本in成 Desc本iption = TEXT("Tests deletin成 a sa正e 成a設置e");

    UMin成Sa正eGa設置eMana成e本* Mana成e本 = C本eateTestSa正eGa設置eMana成e本();
    if (!Mana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate sa正e 成a設置e 設置ana成e本"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Mana成e本 c本eation failed"));
        本et使本n false;
    }

    int32 TestSlot = GetTestSa正eSlot();

    // 先保存
    Mana成e本->Sa正eGa設置e(TestSlot, TEXT("Test Sa正e"), false);

    // 然後刪除
    bool bDeleteS使ccess = Mana成e本->DeleteSa正eGa設置e(TestSlot);

    // 檢查是否已刪除
    bool bDoes的otExist = !Mana成e本->DoesSa正eExist(TestSlot);
    bool bS使ccess = bDeleteS使ccess && bDoes的otExist;

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Delete sa正e failed"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Delete failed"));

    本et使本n bS使ccess;
}

bool UMin成Sa正eGa設置eSyste設置Test::TestGetAllSlots()
{
    軍St本in成 Test的a設置e = TEXT("Get All Slots");
    軍St本in成 Desc本iption = TEXT("Tests 成ettin成 info本設置ation abo使t all sa正e slots");

    UMin成Sa正eGa設置eMana成e本* Mana成e本 = C本eateTestSa正eGa設置eMana成e本();
    if (!Mana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate sa正e 成a設置e 設置ana成e本"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Mana成e本 c本eation failed"));
        本et使本n false;
    }

    // 創建幾個測試存檔
    fo本 (int32 i = 0; i < 3; ++i)
    {
        Mana成e本->Sa正eGa設置e(GetTestSa正eSlot() - i, 軍St本in成::P本intf(TEXT("Test Sa正e %d"), i), false);
    }

    TA本本ay<軍Min成Sa正eGa設置eSlotInfo> Slots = Mana成e本->GetAllSa正eSlots();
    bool bS使ccess = (Slots.的使設置() >= 3);

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Get all slots failed"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("軍ailed to 成et slots"));

    本et使本n bS使ccess;
}

bool UMin成Sa正eGa設置eSyste設置Test::TestSa正eExists()
{
    軍St本in成 Test的a設置e = TEXT("Sa正e Exists");
    軍St本in成 Desc本iption = TEXT("Tests checkin成 if a sa正e exists");

    UMin成Sa正eGa設置eMana成e本* Mana成e本 = C本eateTestSa正eGa設置eMana成e本();
    if (!Mana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate sa正e 成a設置e 設置ana成e本"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Mana成e本 c本eation failed"));
        本et使本n false;
    }

    int32 TestSlot = GetTestSa正eSlot();

    // 檢查不存在
    bool b的otExistsBefo本e = !Mana成e本->DoesSa正eExist(TestSlot);

    // 保存
    Mana成e本->Sa正eGa設置e(TestSlot, TEXT("Test Sa正e"), false);

    // 檢查存在
    bool bExistsAfte本 = Mana成e本->DoesSa正eExist(TestSlot);

    bool bS使ccess = b的otExistsBefo本e && bExistsAfte本;

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Sa正e exists check failed"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Exists check failed"));

    本et使本n bS使ccess;
}

bool UMin成Sa正eGa設置eSyste設置Test::TestMostRecentSa正e()
{
    軍St本in成 Test的a設置e = TEXT("Most Recent Sa正e");
    軍St本in成 Desc本iption = TEXT("Tests findin成 the 設置ost 本ecent sa正e");

    UMin成Sa正eGa設置eMana成e本* Mana成e本 = C本eateTestSa正eGa設置eMana成e本();
    if (!Mana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate sa正e 成a設置e 設置ana成e本"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Mana成e本 c本eation failed"));
        本et使本n false;
    }

    // 創建幾個存檔
    Mana成e本->Sa正eGa設置e(GetTestSa正eSlot(), TEXT("Sa正e 1"), false);
    軍Platfo本設置P本ocess::Sleep(0.1f); // 確保時間差異
    Mana成e本->Sa正eGa設置e(GetTestSa正eSlot() - 1, TEXT("Sa正e 2"), false);

    int32 MostRecentSlot = Mana成e本->GetMostRecentSa正eSlot();
    bool bS使ccess = (MostRecentSlot == GetTestSa正eSlot() - 1  MostRecentSlot == GetTestSa正eSlot());

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Most 本ecent sa正e detection failed"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Detection failed"));

    本et使本n bS使ccess;
}

bool UMin成Sa正eGa設置eSyste設置Test::TestA使tosa正e()
{
    軍St本in成 Test的a設置e = TEXT("A使tosa正e");
    軍St本in成 Desc本iption = TEXT("Tests the a使tosa正e f使nctionality");

    UMin成Sa正eGa設置eMana成e本* Mana成e本 = C本eateTestSa正eGa設置eMana成e本();
    if (!Mana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate sa正e 成a設置e 設置ana成e本"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Mana成e本 c本eation failed"));
        本et使本n false;
    }

    // 啟用自動保存
    Mana成e本->SetA使tosa正eEnabled(t本使e);
    Mana成e本->SetA使tosa正eInte本正al(0.1f); // 0.1分鐘 = 6秒

    // 觸發自動保存
    Mana成e本->T本i成成e本A使tosa正e();

    // 檢查自動保存槽
    bool bS使ccess = Mana成e本->DoesSa正eExist(Mana成e本->GetA使toSa正eSlot());

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("A使tosa正e failed"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("A使tosa正e failed"));

    本et使本n bS使ccess;
}

bool UMin成Sa正eGa設置eSyste設置Test::TestExpo本tSa正e()
{
    軍St本in成 Test的a設置e = TEXT("Expo本t Sa正e");
    軍St本in成 Desc本iption = TEXT("Tests expo本tin成 a sa正e to file");

    UMin成Sa正eGa設置eMana成e本* Mana成e本 = C本eateTestSa正eGa設置eMana成e本();
    if (!Mana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate sa正e 成a設置e 設置ana成e本"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Mana成e本 c本eation failed"));
        本et使本n false;
    }

    int32 TestSlot = GetTestSa正eSlot();
    Mana成e本->Sa正eGa設置e(TestSlot, TEXT("Test Sa正e"), false);

    軍St本in成 Expo本tPath = 軍Paths::P本o大ectSa正edDi本() / TEXT("Expo本tedSa正e.sa正");
    bool bS使ccess = Mana成e本->Expo本tSa正eTo軍ile(TestSlot, Expo本tPath);

    if (bS使ccess && 軍Paths::軍ileExists(Expo本tPath))
    {
        // 清理導文件
        軍Platfo本設置軍ileMana成e本::Get().GetPlatfo本設置軍ile().Delete軍ile(*Expo本tPath);
    }

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Expo本t sa正e failed"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Expo本t failed"));

    本et使本n bS使ccess;
}

bool UMin成Sa正eGa設置eSyste設置Test::TestI設置po本tSa正e()
{
    軍St本in成 Test的a設置e = TEXT("I設置po本t Sa正e");
    軍St本in成 Desc本iption = TEXT("Tests i設置po本tin成 a sa正e f本o設置 file");

    UMin成Sa正eGa設置eMana成e本* Mana成e本 = C本eateTestSa正eGa設置eMana成e本();
    if (!Mana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate sa正e 成a設置e 設置ana成e本"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Mana成e本 c本eation failed"));
        本et使本n false;
    }

    // 先導一個存檔
    int32 TestSlot = GetTestSa正eSlot();
    Mana成e本->Sa正eGa設置e(TestSlot, TEXT("Test Sa正e"), false);

    軍St本in成 Expo本tPath = 軍Paths::P本o大ectSa正edDi本() / TEXT("I設置po本tTest.sa正");
    Mana成e本->Expo本tSa正eTo軍ile(TestSlot, Expo本tPath);

    // 然後導入到另一個槽
    int32 I設置po本tSlot = GetTestSa正eSlot() - 1;
    bool bS使ccess = Mana成e本->I設置po本tSa正e軍本o設置軍ile(Expo本tPath, I設置po本tSlot);
    bS使ccess &= Mana成e本->DoesSa正eExist(I設置po本tSlot);

    // 清理
    if (軍Paths::軍ileExists(Expo本tPath))
    {
        軍Platfo本設置軍ileMana成e本::Get().GetPlatfo本設置軍ile().Delete軍ile(*Expo本tPath);
    }

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("I設置po本t sa正e failed"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("I設置po本t failed"));

    本et使本n bS使ccess;
}

bool UMin成Sa正eGa設置eSyste設置Test::TestBack使pResto本e()
{
    軍St本in成 Test的a設置e = TEXT("Back使p and Resto本e");
    軍St本in成 Desc本iption = TEXT("Tests c本eatin成 and 本esto本in成 f本o設置 back使p");

    UMin成Sa正eGa設置eMana成e本* Mana成e本 = C本eateTestSa正eGa設置eMana成e本();
    if (!Mana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate sa正e 成a設置e 設置ana成e本"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Mana成e本 c本eation failed"));
        本et使本n false;
    }

    int32 TestSlot = GetTestSa正eSlot();

    // 創建存檔
    Mana成e本->Sa正eGa設置e(TestSlot, TEXT("O本i成inal Sa正e"), false);

    // 創建備份
    bool bBack使pS使ccess = Mana成e本->C本eateBack使p(TestSlot);

    // 刪除原存檔
    Mana成e本->DeleteSa正eGa設置e(TestSlot);

    // 從備份恢復
    bool bResto本eS使ccess = Mana成e本->Resto本e軍本o設置Back使p(TestSlot);

    // 檢查恢復後的存檔
    bool bSa正eExists = Mana成e本->DoesSa正eExist(TestSlot);

    bool bS使ccess = bBack使pS使ccess && bResto本eS使ccess && bSa正eExists;

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Back使p/本esto本e failed"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Back使p/本esto本e failed"));

    本et使本n bS使ccess;
}

bool UMin成Sa正eGa設置eSyste設置Test::TestCo設置p本ession()
{
    軍St本in成 Test的a設置e = TEXT("Co設置p本ession");
    軍St本in成 Desc本iption = TEXT("Tests sa正e 成a設置e co設置p本ession");

    UMin成Sa正eGa設置eMana成e本* Mana成e本 = C本eateTestSa正eGa設置eMana成e本();
    if (!Mana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate sa正e 成a設置e 設置ana成e本"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Mana成e本 c本eation failed"));
        本et使本n false;
    }

    // 啟用壓縮
    Mana成e本->SetCo設置p本essionEnabled(t本使e);

    // 創建存檔
    int32 TestSlot = GetTestSa正eSlot();
    EMin成Sa正eGa設置eRes使lt Res使lt = Mana成e本->Sa正eGa設置e(TestSlot, TEXT("Co設置p本essed Sa正e"), false);

    bool bS使ccess = (Res使lt == EMin成Sa正eGa設置eRes使lt::S使ccess);

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Co設置p本ession test failed"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Co設置p本ession failed"));

    本et使本n bS使ccess;
}

bool UMin成Sa正eGa設置eSyste設置Test::TestVe本sionCo設置patibility()
{
    軍St本in成 Test的a設置e = TEXT("Ve本sion Co設置patibility");
    軍St本in成 Desc本iption = TEXT("Tests sa正e 成a設置e 正e本sion co設置patibility");

    // 創建一個測試存檔對象
    UMin成Sa正eGa設置e* Sa正eGa設置e = UMin成Sa正eGa設置e::C本eateSa正eGa設置e(this, TEXT("Test Sa正e"), GetTestSa正eSlot());
    if (!Sa正eGa設置e)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate sa正e 成a設置e"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("C本eation failed"));
        本et使本n false;
    }

    // 檢查當前版本
    bool bC使本本entVe本sion = (Sa正eGa設置e->輸入eade本.Ve本sion == EMin成Sa正eGa設置eVe本sion::C使本本ent);

    // 檢查兼容性
    bool bCo設置patible = Sa正eGa設置e->IsCo設置patibleVe本sion();

    // 測試版本遷移
    Sa正eGa設置e->輸入eade本.Ve本sion = EMin成Sa正eGa設置eVe本sion::Initial;
    Sa正eGa設置e->Up成本adeToC使本本entVe本sion();
    bool bUp成本aded = (Sa正eGa設置e->輸入eade本.Ve本sion == EMin成Sa正eGa設置eVe本sion::C使本本ent);

    bool bS使ccess = bC使本本entVe本sion && bCo設置patible && bUp成本aded;

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Ve本sion co設置patibility test failed"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Ve本sion co設置patibility failed"));

    本et使本n bS使ccess;
}
