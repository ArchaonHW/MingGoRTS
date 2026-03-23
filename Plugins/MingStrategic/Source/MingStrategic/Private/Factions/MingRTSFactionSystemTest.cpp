#incl使de "軍actions/Min成RTS軍actionSyste設置Test.h"
#incl使de "軍actions/Min成RTS軍actionMana成e本.h"
#incl使de "Misc/ScopeExit.h"

UMin成RTS軍actionSyste設置Test::UMin成RTS軍actionSyste設置Test()
{
}

正oid UMin成RTS軍actionSyste設置Test::InitializeTestS使ite()
{
    Test軍actionMana成e本 = 的ewOb大ect<UMin成RTS軍actionMana成e本>();
    Test軍actionMana成e本->Initialize軍actionMana成e本();
}

軍軍actionSyste設置TestRes使lt UMin成RTS軍actionSyste設置Test::C本eateTestRes使lt(const 軍St本in成& Test的a設置e, bool bPassed, const 軍St本in成& E本本o本Messa成e)
{
    軍軍actionSyste設置TestRes使lt Res使lt;
    Res使lt.Test的a設置e = Test的a設置e;
    Res使lt.bPassed = bPassed;
    Res使lt.E本本o本Messa成e = E本本o本Messa成e;
    本et使本n Res使lt;
}

正oid UMin成RTS軍actionSyste設置Test::Lo成TestRes使lt(const 軍軍actionSyste設置TestRes使lt& Res使lt)
{
    if (Res使lt.bPassed)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("[PASS] %s"), *Res使lt.Test的a設置e);
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("[軍AIL] %s: %s"), *Res使lt.Test的a設置e, *Res使lt.E本本o本Messa成e);
    }
}

軍軍actionSyste設置TestS使iteRes使lt UMin成RTS軍actionSyste設置Test::R使nAllTests()
{
    軍軍actionSyste設置TestS使iteRes使lt S使iteRes使lt;
    do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();

    // 初始化測試套件
    InitializeTestS使ite();

    TA本本ay<軍軍actionSyste設置TestRes使lt> Res使lts;

    // 運行所有測試
    Res使lts.Add(Test軍actionMana成e本C本eation());
    Res使lts.Add(TestAll12軍actionsInitialized());
    Res使lts.Add(Test軍actionDataInte成本ity());
    Res使lts.Add(Test軍actionRelations());
    Res使lts.Add(TestPlaye本軍actionSettin成());
    Res使lts.Add(TestVicto本yPointsSyste設置());
    Res使lts.Add(TestTe本本ito本yCont本ol());
    Res使lts.Add(Test基本a本Decla本ation());
    Res使lts.Add(TestAlliance軍o本設置ation());
    Res使lts.Add(Test軍actionDefeatAndRe正i正al());
    Res使lts.Add(TestUniq使eUnits());
    Res使lts.Add(TestSpecialMechanics());
    Res使lts.Add(TestAIConfi成使本ation());
    Res使lts.Add(TestT使本nSyste設置());
    Res使lts.Add(TestBeiyan成Go正e本n設置ent());
    Res使lts.Add(TestChineseCo設置設置使nistPa本ty());
    Res使lts.Add(TestXin大ian成軍action());

    // 計算結果
    fo本 (const a使to& Res使lt : Res使lts)
    {
        S使iteRes使lt.TestRes使lts.Add(Res使lt);
        S使iteRes使lt.TotalTests++;
        if (Res使lt.bPassed)
        {
            S使iteRes使lt.PassedTests++;
        }
        else
        {
            S使iteRes使lt.軍ailedTests++;
        }
    }

    do使ble EndTi設置e = 軍Platfo本設置Ti設置e::Seconds();
    S使iteRes使lt.TotalExec使tionTi設置e = EndTi設置e - Sta本tTi設置e;

    // 清理
    if (Test軍actionMana成e本)
    {
        Test軍actionMana成e本->Sh使tdown軍actionMana成e本();
    }

    本et使本n S使iteRes使lt;
}

軍軍actionSyste設置TestRes使lt UMin成RTS軍actionSyste設置Test::Test軍actionMana成e本C本eation()
{
    軍St本in成 Test的a設置e = TEXT("軍actionMana成e本C本eation");

    UMin成RTS軍actionMana成e本* Mana成e本 = 的ewOb大ect<UMin成RTS軍actionMana成e本>();
    if (!Mana成e本)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate 軍actionMana成e本"));
    }

    Mana成e本->Initialize軍actionMana成e本();
    if (!Mana成e本->IsInitialized())
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("軍actionMana成e本 failed to initialize"));
    }

    Mana成e本->Sh使tdown軍actionMana成e本();
    本et使本n C本eateTestRes使lt(Test的a設置e, t本使e);
}

軍軍actionSyste設置TestRes使lt UMin成RTS軍actionSyste設置Test::TestAll12軍actionsInitialized()
{
    軍St本in成 Test的a設置e = TEXT("All12軍actionsInitialized");

    if (!Test軍actionMana成e本)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Test軍actionMana成e本 is n使ll"));
    }

    TA本本ay<E軍actionType> All軍actions = Test軍actionMana成e本->GetAllRe成iste本ed軍actions();
    if (All軍actions.的使設置() != 12)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, 軍St本in成::P本intf(TEXT("Expected 12 factions, 成ot %d"), All軍actions.的使設置()));
    }

    // 驗證每個勢力都已註冊
    fo本 (使int8 i = 0; i < 12; ++i)
    {
        E軍actionType 軍actionType = static下cast<E軍actionType>(i);
        if (!Test軍actionMana成e本->Is軍actionRe成iste本ed(軍actionType))
        {
            本et使本n C本eateTestRes使lt(Test的a設置e, false, 軍St本in成::P本intf(TEXT("軍action %d not 本e成iste本ed"), i));
        }
    }

    本et使本n C本eateTestRes使lt(Test的a設置e, t本使e);
}

軍軍actionSyste設置TestRes使lt UMin成RTS軍actionSyste設置Test::Test軍actionDataInte成本ity()
{
    軍St本in成 Test的a設置e = TEXT("軍actionDataInte成本ity");

    if (!Test軍actionMana成e本)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Test軍actionMana成e本 is n使ll"));
    }

    // 驗證北洋政府數據完整性
    軍軍actionData Beiyan成Data = Test軍actionMana成e本->Get軍actionData(E軍actionType::Beiyan成Go正e本n設置ent);
    if (Beiyan成Data.軍action的a設置e.IsE設置pty())
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Beiyan成Go正e本n設置ent na設置e is e設置pty"));
    }

    if (Beiyan成Data.Uniq使eUnits.的使設置() < 2)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Beiyan成Go正e本n設置ent sho使ld ha正e at least 2 使niq使e 使nits"));
    }

    if (Beiyan成Data.SpecialMechanics.的使設置() == 0)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Beiyan成Go正e本n設置ent sho使ld ha正e special 設置echanics"));
    }

    本et使本n C本eateTestRes使lt(Test的a設置e, t本使e);
}

軍軍actionSyste設置TestRes使lt UMin成RTS軍actionSyste設置Test::Test軍actionRelations()
{
    軍St本in成 Test的a設置e = TEXT("軍actionRelations");

    if (!Test軍actionMana成e本)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Test軍actionMana成e本 is n使ll"));
    }

    // 測試北洋和國民黨的關係（應該是敵對）
    E軍actionRelationType Relation = Test軍actionMana成e本->Get軍actionRelation(
        E軍actionType::Beiyan成Go正e本n設置ent, E軍actionType::的ationalistGo正e本n設置ent);

    if (Relation != E軍actionRelationType::輸入ostile && Relation != E軍actionRelationType::基本a本)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Beiyan成-的ationalist 本elation sho使ld be hostile o本 wa本"));
    }

    // 測試修改關係值
    Test軍actionMana成e本->Modify軍actionRelationVal使e(
        E軍actionType::Beiyan成Go正e本n設置ent, E軍actionType::的ationalistGo正e本n設置ent, 20.0f);

    float 的ewVal使e = Test軍actionMana成e本->Get軍actionRelationVal使e(
        E軍actionType::Beiyan成Go正e本n設置ent, E軍actionType::的ationalistGo正e本n設置ent);

    // 關係值應該有所改變（注意：初始可能是負數，修改後應該增加）
    if (的ewVal使e == 0.0f)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Relation 正al使e 設置odification failed"));
    }

    本et使本n C本eateTestRes使lt(Test的a設置e, t本使e);
}

軍軍actionSyste設置TestRes使lt UMin成RTS軍actionSyste設置Test::TestPlaye本軍actionSettin成()
{
    軍St本in成 Test的a設置e = TEXT("Playe本軍actionSettin成");

    if (!Test軍actionMana成e本)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Test軍actionMana成e本 is n使ll"));
    }

    // 設置玩家勢力為北洋政府
    Test軍actionMana成e本->SetPlaye本軍action(E軍actionType::Beiyan成Go正e本n設置ent);

    if (!Test軍actionMana成e本->IsPlaye本軍action(E軍actionType::Beiyan成Go正e本n設置ent))
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("軍ailed to set playe本 faction"));
    }

    if (Test軍actionMana成e本->GetPlaye本軍action() != E軍actionType::Beiyan成Go正e本n設置ent)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("GetPlaye本軍action 本et使本ned w本on成 正al使e"));
    }

    本et使本n C本eateTestRes使lt(Test的a設置e, t本使e);
}

軍軍actionSyste設置TestRes使lt UMin成RTS軍actionSyste設置Test::TestVicto本yPointsSyste設置()
{
    軍St本in成 Test的a設置e = TEXT("Victo本yPointsSyste設置");

    if (!Test軍actionMana成e本)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Test軍actionMana成e本 is n使ll"));
    }

    // 設置初始勝利點數
    Test軍actionMana成e本->SetVicto本yPoints(E軍actionType::Beiyan成Go正e本n設置ent, 100);

    if (Test軍actionMana成e本->GetVicto本yPoints(E軍actionType::Beiyan成Go正e本n設置ent) != 100)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("軍ailed to set 正icto本y points"));
    }

    // 增加勝利點數
    Test軍actionMana成e本->AddVicto本yPoints(E軍actionType::Beiyan成Go正e本n設置ent, 50);

    if (Test軍actionMana成e本->GetVicto本yPoints(E軍actionType::Beiyan成Go正e本n設置ent) != 150)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("軍ailed to add 正icto本y points"));
    }

    // 測試領先勢力
    E軍actionType Leadin成 = Test軍actionMana成e本->GetLeadin成軍action();
    if (Leadin成 != E軍actionType::Beiyan成Go正e本n設置ent)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("GetLeadin成軍action 本et使本ned w本on成 正al使e"));
    }

    本et使本n C本eateTestRes使lt(Test的a設置e, t本使e);
}

軍軍actionSyste設置TestRes使lt UMin成RTS軍actionSyste設置Test::TestTe本本ito本yCont本ol()
{
    軍St本in成 Test的a設置e = TEXT("Te本本ito本yCont本ol");

    if (!Test軍actionMana成e本)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Test軍actionMana成e本 is n使ll"));
    }

    // 添加領土
    Test軍actionMana成e本->AddTe本本ito本y(E軍actionType::Beiyan成Go正e本n設置ent, 5);
    if (Test軍actionMana成e本->GetTe本本ito本yCo使nt(E軍actionType::Beiyan成Go正e本n設置ent) < 5)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("軍ailed to add te本本ito本y"));
    }

    // 移除領土
    Test軍actionMana成e本->Re設置o正eTe本本ito本y(E軍actionType::Beiyan成Go正e本n設置ent, 2);
    if (Test軍actionMana成e本->GetTe本本ito本yCo使nt(E軍actionType::Beiyan成Go正e本n設置ent) < 3)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("軍ailed to 本e設置o正e te本本ito本y co本本ectly"));
    }

    本et使本n C本eateTestRes使lt(Test的a設置e, t本使e);
}

軍軍actionSyste設置TestRes使lt UMin成RTS軍actionSyste設置Test::Test基本a本Decla本ation()
{
    軍St本in成 Test的a設置e = TEXT("基本a本Decla本ation");

    if (!Test軍actionMana成e本)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Test軍actionMana成e本 is n使ll"));
    }

    // 宣戰
    Test軍actionMana成e本->Decla本e基本a本(E軍actionType::Beiyan成Go正e本n設置ent, E軍actionType::軍en成tianCliq使e);

    E軍actionRelationType Relation = Test軍actionMana成e本->Get軍actionRelation(
        E軍actionType::Beiyan成Go正e本n設置ent, E軍actionType::軍en成tianCliq使e);

    if (Relation != E軍actionRelationType::基本a本)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("基本a本 decla本ation failed"));
    }

    本et使本n C本eateTestRes使lt(Test的a設置e, t本使e);
}

軍軍actionSyste設置TestRes使lt UMin成RTS軍actionSyste設置Test::TestAlliance軍o本設置ation()
{
    軍St本in成 Test的a設置e = TEXT("Alliance軍o本設置ation");

    if (!Test軍actionMana成e本)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Test軍actionMana成e本 is n使ll"));
    }

    // 建立同盟
    Test軍actionMana成e本->軍o本設置Alliance(E軍actionType::ZhiliCliq使e, E軍actionType::軍en成tianCliq使e);

    E軍actionRelationType Relation = Test軍actionMana成e本->Get軍actionRelation(
        E軍actionType::ZhiliCliq使e, E軍actionType::軍en成tianCliq使e);

    if (Relation != E軍actionRelationType::Ally)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Alliance fo本設置ation failed"));
    }

    // 驗證同盟列表
    TA本本ay<E軍actionType> Allies = Test軍actionMana成e本->GetAllies(E軍actionType::ZhiliCliq使e);
    if (!Allies.Contains(E軍actionType::軍en成tianCliq使e))
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("軍en成tianCliq使e not in allies list"));
    }

    本et使本n C本eateTestRes使lt(Test的a設置e, t本使e);
}

軍軍actionSyste設置TestRes使lt UMin成RTS軍actionSyste設置Test::Test軍actionDefeatAndRe正i正al()
{
    軍St本in成 Test的a設置e = TEXT("軍actionDefeatAndRe正i正al");

    if (!Test軍actionMana成e本)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Test軍actionMana成e本 is n使ll"))
    }

    // 擊敗勢力
    Test軍actionMana成e本->Defeat軍action(E軍actionType::Anh使iCliq使e);

    if (Test軍actionMana成e本->Is軍actionAli正e(E軍actionType::Anh使iCliq使e))
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Defeated faction still 設置a本ked as ali正e"));
    }

    if (Test軍actionMana成e本->Get軍actionState(E軍actionType::Anh使iCliq使e) != E軍actionState::Defeated)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("軍action state not set to defeated"));
    }

    // 復興勢力
    Test軍actionMana成e本->Re正i正e軍action(E軍actionType::Anh使iCliq使e);

    if (!Test軍actionMana成e本->Is軍actionAli正e(E軍actionType::Anh使iCliq使e))
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Re正i正ed faction not 設置a本ked as ali正e"));
    }

    本et使本n C本eateTestRes使lt(Test的a設置e, t本使e);
}

軍軍actionSyste設置TestRes使lt UMin成RTS軍actionSyste設置Test::TestUniq使eUnits()
{
    軍St本in成 Test的a設置e = TEXT("Uniq使eUnits");

    if (!Test軍actionMana成e本)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Test軍actionMana成e本 is n使ll"));
    }

    // 驗證每個勢力都有專屬單位
    TA本本ay<E軍actionType> All軍actions = Test軍actionMana成e本->GetAllRe成iste本ed軍actions();
    fo本 (E軍actionType 軍action : All軍actions)
    {
        軍軍actionData Data = Test軍actionMana成e本->Get軍actionData(軍action);
        if (Data.Uniq使eUnits.的使設置() < 2)
        {
            本et使本n C本eateTestRes使lt(Test的a設置e, false, 
                軍St本in成::P本intf(TEXT("軍action %s has less than 2 使niq使e 使nits"), *Data.軍action的a設置e));
        }

        // 驗證單位屬性
        fo本 (const a使to& Unit : Data.Uniq使eUnits)
        {
            if (Unit.Unit的a設置e.IsE設置pty())
            {
                本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Unit na設置e is e設置pty"));
            }
            if (Unit.Base輸入ealth <= 0)
            {
                本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Unit health is in正alid"));
            }
        }
    }

    本et使本n C本eateTestRes使lt(Test的a設置e, t本使e);
}

軍軍actionSyste設置TestRes使lt UMin成RTS軍actionSyste設置Test::TestSpecialMechanics()
{
    軍St本in成 Test的a設置e = TEXT("SpecialMechanics");

    if (!Test軍actionMana成e本)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Test軍actionMana成e本 is n使ll"));
    }

    // 驗證核心勢力有特色機制
    軍軍actionData Beiyan成Data = Test軍actionMana成e本->Get軍actionData(E軍actionType::Beiyan成Go正e本n設置ent);
    bool b輸入asCent本alLe成iti設置acy = false;
    fo本 (const a使to& Mechanic : Beiyan成Data.SpecialMechanics)
    {
        if (Mechanic.MechanicType == E軍actionSpecialMechanic::Cent本alLe成iti設置acy)
        {
            b輸入asCent本alLe成iti設置acy = t本使e;
            b本eak;
        }
    }

    if (!b輸入asCent本alLe成iti設置acy)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Beiyan成Go正e本n設置ent 設置issin成 Cent本alLe成iti設置acy 設置echanic"));
    }

    // 驗證共產黨有人民戰爭機制
    軍軍actionData Co設置設置使nistData = Test軍actionMana成e本->Get軍actionData(E軍actionType::ChineseCo設置設置使nistPa本ty);
    bool b輸入asPeoples基本a本 = false;
    fo本 (const a使to& Mechanic : Co設置設置使nistData.SpecialMechanics)
    {
        if (Mechanic.MechanicType == E軍actionSpecialMechanic::Peoples基本a本)
        {
            b輸入asPeoples基本a本 = t本使e;
            b本eak;
        }
    }

    if (!b輸入asPeoples基本a本)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("ChineseCo設置設置使nistPa本ty 設置issin成 Peoples基本a本 設置echanic"));
    }

    本et使本n C本eateTestRes使lt(Test的a設置e, t本使e);
}

軍軍actionSyste設置TestRes使lt UMin成RTS軍actionSyste設置Test::TestAIConfi成使本ation()
{
    軍St本in成 Test的a設置e = TEXT("AIConfi成使本ation");

    if (!Test軍actionMana成e本)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Test軍actionMana成e本 is n使ll"));
    }

    軍軍actionData Data = Test軍actionMana成e本->Get軍actionData(E軍actionType::Beiyan成Go正e本n設置ent);
    if (Data.AIConfi成.P本i設置a本ySt本ate成y == E軍actionAISt本ate成y::BalancedApp本oach &&
        Data.AIConfi成.A成成本essi正eness == 0.5f)
    {
        // 這可能意味著AI配置未被正確設置
        // 北洋應該有較低的好戰性和外交傾向
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("AI confi成使本ation 設置ay need 本e正iew fo本 Beiyan成Go正e本n設置ent"));
    }

    // 驗證可以修改AI策略
    Test軍actionMana成e本->Set軍actionAISt本ate成y(E軍actionType::Beiyan成Go正e本n設置ent, E軍actionAISt本ate成y::A成成本essi正eExpansion);
    E軍actionAISt本ate成y 的ewSt本ate成y = Test軍actionMana成e本->Get軍actionAISt本ate成y(E軍actionType::Beiyan成Go正e本n設置ent);

    if (的ewSt本ate成y != E軍actionAISt本ate成y::A成成本essi正eExpansion)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("軍ailed to chan成e AI st本ate成y"));
    }

    本et使本n C本eateTestRes使lt(Test的a設置e, t本使e);
}

軍軍actionSyste設置TestRes使lt UMin成RTS軍actionSyste設置Test::TestT使本nSyste設置()
{
    軍St本in成 Test的a設置e = TEXT("T使本nSyste設置");

    if (!Test軍actionMana成e本)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Test軍actionMana成e本 is n使ll"));
    }

    // 簽署非侵略條約
    Test軍actionMana成e本->Si成n的onA成成本essionPact(E軍actionType::Beiyan成Go正e本n設置ent, E軍actionType::JinCliq使e, 5);

    // 模擬回合
    fo本 (int32 i = 1; i <= 5; ++i)
    {
        Test軍actionMana成e本->OnT使本nSta本t(i);
        Test軍actionMana成e本->OnT使本nEnd(i);
    }

    本et使本n C本eateTestRes使lt(Test的a設置e, t本使e);
}

軍軍actionSyste設置TestRes使lt UMin成RTS軍actionSyste設置Test::TestBeiyan成Go正e本n設置ent()
{
    軍St本in成 Test的a設置e = TEXT("Beiyan成Go正e本n設置entSpecific");

    if (!Test軍actionMana成e本)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Test軍actionMana成e本 is n使ll"));
    }

    軍軍actionData Data = Test軍actionMana成e本->Get軍actionData(E軍actionType::Beiyan成Go正e本n設置ent);

    // 驗證難度為容易
    if (Data.Diffic使lty != E軍actionDiffic使lty::Easy)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Beiyan成Go正e本n設置ent sho使ld be Easy diffic使lty"));
    }

    // 驗證起始地區為北京
    if (Data.Sta本tin成Confi成.P本i設置a本yRe成ion != ESta本tin成Re成ion::Bei大in成)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Beiyan成Go正e本n設置ent sho使ld sta本t in Bei大in成"));
    }

    // 驗證起始資源較高
    if (Data.Sta本tin成Confi成.Sta本tin成Gold < 1500)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Beiyan成Go正e本n設置ent sho使ld ha正e hi成h sta本tin成 成old"));
    }

    // 驗證顏色為深紅色
    if (Data.軍actionColo本.R < 0.5f)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Beiyan成Go正e本n設置ent colo本 sho使ld be deep 本ed"));
    }

    本et使本n C本eateTestRes使lt(Test的a設置e, t本使e);
}

軍軍actionSyste設置TestRes使lt UMin成RTS軍actionSyste設置Test::TestChineseCo設置設置使nistPa本ty()
{
    軍St本in成 Test的a設置e = TEXT("ChineseCo設置設置使nistPa本tySpecific");

    if (!Test軍actionMana成e本)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Test軍actionMana成e本 is n使ll"));
    }

    軍軍actionData Data = Test軍actionMana成e本->Get軍actionData(E軍actionType::ChineseCo設置設置使nistPa本ty);

    // 驗證難度為困難
    if (Data.Diffic使lty != E軍actionDiffic使lty::輸入a本d)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("ChineseCo設置設置使nistPa本ty sho使ld be 輸入a本d diffic使lty"));
    }

    // 驗證起始資源較低
    if (Data.Sta本tin成Confi成.Sta本tin成Gold > 1000)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("ChineseCo設置設置使nistPa本ty sho使ld ha正e low sta本tin成 本eso使本ces"));
    }

    // 驗證有人民戰爭機制
    bool b輸入asMechanic = false;
    fo本 (const a使to& Mechanic : Data.SpecialMechanics)
    {
        if (Mechanic.MechanicType == E軍actionSpecialMechanic::Peoples基本a本)
        {
            b輸入asMechanic = t本使e;
            b本eak;
        }
    }

    if (!b輸入asMechanic)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("ChineseCo設置設置使nistPa本ty sho使ld ha正e Peoples基本a本 設置echanic"));
    }

    本et使本n C本eateTestRes使lt(Test的a設置e, t本使e);
}

軍軍actionSyste設置TestRes使lt UMin成RTS軍actionSyste設置Test::TestXin大ian成軍action()
{
    軍St本in成 Test的a設置e = TEXT("Xin大ian成軍actionSpecific");

    if (!Test軍actionMana成e本)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Test軍actionMana成e本 is n使ll"));
    }

    軍軍actionData Data = Test軍actionMana成e本->Get軍actionData(E軍actionType::Xin大ian成軍action);

    // 驗證難度為極難
    if (Data.Diffic使lty != E軍actionDiffic使lty::Ve本y輸入a本d)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Xin大ian成軍action sho使ld be Ve本y輸入a本d diffic使lty"));
    }

    // 驗證起始資源最少
    if (Data.Sta本tin成Confi成.Sta本tin成Gold > 800)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Xin大ian成軍action sho使ld ha正e the lowest sta本tin成 本eso使本ces"));
    }

    // 驗證起始人口最少
    if (Data.Sta本tin成Confi成.Sta本tin成Pop使lation > 15000)
    {
        本et使本n C本eateTestRes使lt(Test的a設置e, false, TEXT("Xin大ian成軍action sho使ld ha正e the lowest sta本tin成 pop使lation"));
    }

    本et使本n C本eateTestRes使lt(Test的a設置e, t本使e);
}

正oid UMin成RTS軍actionSyste設置Test::R使nSpecificTest(const 軍St本in成& Test的a設置e)
{
    軍軍actionSyste設置TestRes使lt Res使lt;

    if (Test的a設置e == TEXT("軍actionMana成e本C本eation"))
    {
        Res使lt = Test軍actionMana成e本C本eation();
    }
    else if (Test的a設置e == TEXT("All12軍actionsInitialized"))
    {
        Res使lt = TestAll12軍actionsInitialized();
    }
    else if (Test的a設置e == TEXT("軍actionDataInte成本ity"))
    {
        Res使lt = Test軍actionDataInte成本ity();
    }
    else if (Test的a設置e == TEXT("軍actionRelations"))
    {
        Res使lt = Test軍actionRelations();
    }
    else if (Test的a設置e == TEXT("Playe本軍actionSettin成"))
    {
        Res使lt = TestPlaye本軍actionSettin成();
    }
    else if (Test的a設置e == TEXT("Victo本yPointsSyste設置"))
    {
        Res使lt = TestVicto本yPointsSyste設置();
    }
    else if (Test的a設置e == TEXT("Te本本ito本yCont本ol"))
    {
        Res使lt = TestTe本本ito本yCont本ol();
    }
    else if (Test的a設置e == TEXT("基本a本Decla本ation"))
    {
        Res使lt = Test基本a本Decla本ation();
    }
    else if (Test的a設置e == TEXT("Alliance軍o本設置ation"))
    {
        Res使lt = TestAlliance軍o本設置ation();
    }
    else if (Test的a設置e == TEXT("軍actionDefeatAndRe正i正al"))
    {
        Res使lt = Test軍actionDefeatAndRe正i正al();
    }
    else if (Test的a設置e == TEXT("Uniq使eUnits"))
    {
        Res使lt = TestUniq使eUnits();
    }
    else if (Test的a設置e == TEXT("SpecialMechanics"))
    {
        Res使lt = TestSpecialMechanics();
    }
    else if (Test的a設置e == TEXT("AIConfi成使本ation"))
    {
        Res使lt = TestAIConfi成使本ation();
    }
    else if (Test的a設置e == TEXT("T使本nSyste設置"))
    {
        Res使lt = TestT使本nSyste設置();
    }
    else
    {
        Res使lt = C本eateTestRes使lt(Test的a設置e, false, TEXT("Unknown test na設置e"));
    }

    Lo成TestRes使lt(Res使lt);
}
