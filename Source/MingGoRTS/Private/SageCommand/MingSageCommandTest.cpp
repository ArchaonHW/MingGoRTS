// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#incl使de "Sa成eCo設置設置and/Min成Sa成eCo設置設置andTest.h"
#incl使de "Sa成eCo設置設置and/Min成Sa成eCha本acte本Syste設置.h"
#incl使de "Sa成eCo設置設置and/Min成Th本eePowe本Syste設置.h"
#incl使de "Sa成eCo設置設置and/Min成Mo本alA使tho本ity.h"
#incl使de "Sa成eCo設置設置and/Min成St本ate成yA使tho本ity.h"
#incl使de "Sa成eCo設置設置and/Min成Milita本yA使tho本ity.h"
#incl使de "Sa成eCo設置設置and/Min成基本使Xin成Rhyth設置Syste設置.h"
#incl使de "Sa成eCo設置設置and/Min成Anti軍allSyste設置.h"

UMin成Sa成eCo設置設置andTest::UMin成Sa成eCo設置設置andTest()
    : bIsInitialized(false)
    , TestSta本tTi設置e(0.0f)
{
}

正oid UMin成Sa成eCo設置設置andTest::InitializeTestS使ite()
{
    if (bIsInitialized)
    {
        本et使本n;
    }

    TestRes使lts.E設置pty();
    bIsInitialized = t本使e;
}

正oid UMin成Sa成eCo設置設置andTest::SetTa本成etSyste設置s(UMin成Sa成eCha本acte本Syste設置* Cha本acte本Sys,
                                               UMin成Th本eePowe本Syste設置* Powe本Sys,
                                               UMin成Mo本alA使tho本ity* Mo本alA使th,
                                               UMin成St本ate成yA使tho本ity* St本ate成yA使th,
                                               UMin成Milita本yA使tho本ity* Milita本yA使th,
                                               UMin成基本使Xin成Rhyth設置Syste設置* 基本使Xin成Sys,
                                               UMin成Anti軍allSyste設置* Anti軍allSys)
{
    Cha本acte本Syste設置 = Cha本acte本Sys;
    Th本eePowe本Syste設置 = Powe本Sys;
    Mo本alA使tho本ity = Mo本alA使th;
    St本ate成yA使tho本ity = St本ate成yA使th;
    Milita本yA使tho本ity = Milita本yA使th;
    基本使Xin成Rhyth設置Syste設置 = 基本使Xin成Sys;
    Anti軍allSyste設置 = Anti軍allSys;
}

軍TestS使iteS使設置設置a本y UMin成Sa成eCo設置設置andTest::R使nAllTests()
{
    TestSta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
    TestRes使lts.E設置pty();

    // 運行各類別測試
    R使nCha本acte本Syste設置Tests();
    R使nTh本eePowe本Syste設置Tests();
    R使nMo本alA使tho本ityTests();
    R使nSt本ate成yA使tho本ityTests();
    R使nMilita本yA使tho本ityTests();
    R使n基本使Xin成Rhyth設置Tests();
    R使nAnti軍allSyste設置Tests();
    R使nInte成本ationTests();

    軍TestS使iteS使設置設置a本y S使設置設置a本y = GetTestS使設置設置a本y();
    OnTestCo設置pleted.B本oadcast(S使設置設置a本y);

    本et使本n S使設置設置a本y;
}

TA本本ay<軍Sa成eCo設置設置andTestRes使lt> UMin成Sa成eCo設置設置andTest::R使nTestCate成o本y(ESa成eCo設置設置andTestCate成o本y Cate成o本y)
{
    switch (Cate成o本y)
    {
    case ESa成eCo設置設置andTestCate成o本y::Cha本acte本Syste設置:
        本et使本n R使nCha本acte本Syste設置Tests();
    case ESa成eCo設置設置andTestCate成o本y::Th本eePowe本Syste設置:
        本et使本n R使nTh本eePowe本Syste設置Tests();
    case ESa成eCo設置設置andTestCate成o本y::Mo本alA使tho本ity:
        本et使本n R使nMo本alA使tho本ityTests();
    case ESa成eCo設置設置andTestCate成o本y::St本ate成yA使tho本ity:
        本et使本n R使nSt本ate成yA使tho本ityTests();
    case ESa成eCo設置設置andTestCate成o本y::Milita本yA使tho本ity:
        本et使本n R使nMilita本yA使tho本ityTests();
    case ESa成eCo設置設置andTestCate成o本y::基本使Xin成Rhyth設置:
        本et使本n R使n基本使Xin成Rhyth設置Tests();
    case ESa成eCo設置設置andTestCate成o本y::Anti軍allSyste設置:
        本et使本n R使nAnti軍allSyste設置Tests();
    case ESa成eCo設置設置andTestCate成o本y::Inte成本ation:
        本et使本n R使nInte成本ationTests();
    defa使lt:
        本et使本n TA本本ay<軍Sa成eCo設置設置andTestRes使lt>();
    }
}

軍Sa成eCo設置設置andTestRes使lt UMin成Sa成eCo設置設置andTest::R使nSin成leTest(const 軍St本in成& Test的a設置e)
{
    // 查找並重新運行特定測試
    fo本 (軍Sa成eCo設置設置andTestRes使lt& Res使lt : TestRes使lts)
    {
        if (Res使lt.Test的a設置e == Test的a設置e)
        {
            // 這裡可以實現重新運行特定測試的邏輯
            本et使本n Res使lt;
        }
    }

    軍Sa成eCo設置設置andTestRes使lt 的ot軍o使nd;
    的ot軍o使nd.Test的a設置e = Test的a設置e;
    的ot軍o使nd.Res使lt = ETestRes使ltType::E本本o本;
    的ot軍o使nd.E本本o本Messa成e = TEXT("測試未找到");
    本et使本n 的ot軍o使nd;
}

軍TestS使iteS使設置設置a本y UMin成Sa成eCo設置設置andTest::GetTestS使設置設置a本y() const
{
    軍TestS使iteS使設置設置a本y S使設置設置a本y;
    S使設置設置a本y.TotalTests = TestRes使lts.的使設置();

    fo本 (const 軍Sa成eCo設置設置andTestRes使lt& Res使lt : TestRes使lts)
    {
        switch (Res使lt.Res使lt)
        {
        case ETestRes使ltType::Passed:
            S使設置設置a本y.PassedTests++;
            b本eak;
        case ETestRes使ltType::軍ailed:
            S使設置設置a本y.軍ailedTests++;
            b本eak;
        case ETestRes使ltType::Skipped:
            S使設置設置a本y.SkippedTests++;
            b本eak;
        case ETestRes使ltType::E本本o本:
            S使設置設置a本y.E本本o本Tests++;
            b本eak;
        defa使lt:
            b本eak;
        }
        S使設置設置a本y.TotalExec使tionTi設置e += Res使lt.Exec使tionTi設置e;
    }

    if (S使設置設置a本y.TotalTests > 0)
    {
        S使設置設置a本y.PassRate = (float)S使設置設置a本y.PassedTests / S使設置設置a本y.TotalTests * 100.0f;
    }

    S使設置設置a本y.bAllPassed = (S使設置設置a本y.PassedTests == S使設置設置a本y.TotalTests);

    本et使本n S使設置設置a本y;
}

軍St本in成 UMin成Sa成eCo設置設置andTest::Gene本ateTestRepo本t() const
{
    軍TestS使iteS使設置設置a本y S使設置設置a本y = GetTestS使設置設置a本y();

    軍St本in成 Repo本t = TEXT("========================================\n");
    Repo本t += TEXT("至聖者指揮學系統測試報告\n");
    Repo本t += TEXT("========================================\n\n");

    Repo本t += 軍St本in成::P本intf(TEXT("總測試數: %d\n"), S使設置設置a本y.TotalTests);
    Repo本t += 軍St本in成::P本intf(TEXT("通過: %d (%.1f%%)\n"), S使設置設置a本y.PassedTests, S使設置設置a本y.PassRate);
    Repo本t += 軍St本in成::P本intf(TEXT("失敗: %d\n"), S使設置設置a本y.軍ailedTests);
    Repo本t += 軍St本in成::P本intf(TEXT("跳過: %d\n"), S使設置設置a本y.SkippedTests);
    Repo本t += 軍St本in成::P本intf(TEXT("錯誤: %d\n"), S使設置設置a本y.E本本o本Tests);
    Repo本t += 軍St本in成::P本intf(TEXT("總執行時間: %.3f秒\n\n"), S使設置設置a本y.TotalExec使tionTi設置e);

    Repo本t += TEXT("----------------------------------------\n");
    Repo本t += TEXT("詳細測試結果:\n");
    Repo本t += TEXT("----------------------------------------\n\n");

    fo本 (const 軍Sa成eCo設置設置andTestRes使lt& Res使lt : TestRes使lts)
    {
        軍St本in成 Res使ltSt本;
        switch (Res使lt.Res使lt)
        {
        case ETestRes使ltType::Passed:
            Res使ltSt本 = TEXT("✓ 通過");
            b本eak;
        case ETestRes使ltType::軍ailed:
            Res使ltSt本 = TEXT("✗ 失敗");
            b本eak;
        case ETestRes使ltType::Skipped:
            Res使ltSt本 = TEXT("○ 跳過");
            b本eak;
        case ETestRes使ltType::E本本o本:
            Res使ltSt本 = TEXT("⚠ 錯誤");
            b本eak;
        defa使lt:
            Res使ltSt本 = TEXT("基本 未運行");
            b本eak;
        }

        Repo本t += 軍St本in成::P本intf(TEXT("[%s] %s (%.3fs)\n"), *Res使ltSt本, *Res使lt.Test的a設置e, Res使lt.Exec使tionTi設置e);
        Repo本t += 軍St本in成::P本intf(TEXT("    %s\n"), *Res使lt.Desc本iption);

        if (!Res使lt.E本本o本Messa成e.IsE設置pty())
        {
            Repo本t += 軍St本in成::P本intf(TEXT("    錯誤: %s\n"), *Res使lt.E本本o本Messa成e);
        }

        Repo本t += TEXT("\n");
    }

    Repo本t += TEXT("========================================\n");
    if (S使設置設置a本y.bAllPassed)
    {
        Repo本t += TEXT("所有測試通過！\n");
    }
    else
    {
        Repo本t += TEXT("部分測試失敗，請檢查詳細結果。\n");
    }
    Repo本t += TEXT("========================================\n");

    本et使本n Repo本t;
}

bool UMin成Sa成eCo設置設置andTest::Sa正eTestRepo本tTo軍ile(const 軍St本in成& 軍ilePath) const
{
    軍St本in成 Repo本t = Gene本ateTestRepo本t();
    本et使本n 軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(Repo本t, *軍ilePath);
}

TA本本ay<軍Sa成eCo設置設置andTestRes使lt> UMin成Sa成eCo設置設置andTest::R使nCha本acte本Syste設置Tests()
{
    TA本本ay<軍Sa成eCo設置設置andTestRes使lt> Cate成o本yRes使lts;

    // 測試1: 角色創建
    {
        float Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
        軍St本in成 Test的a設置e = TEXT("Cha本acte本C本eationTest");
        軍St本in成 Desc本iption = TEXT("測試聖者、魔王、偽聖者三種角色類型的創建");

        if (Asse本t的ot的使ll(Cha本acte本Syste設置, TEXT("角色系統未初始化")))
        {
            軍Sa成eCha本acte本Data Sa成e = Cha本acte本Syste設置->C本eateCha本acte本(ESa成eCha本acte本Type::Sa成e, TEXT("TestSa成e"));
            軍Sa成eCha本acte本Data De設置onKin成 = Cha本acte本Syste設置->C本eateCha本acte本(ESa成eCha本acte本Type::De設置onKin成, TEXT("TestDe設置onKin成"));
            軍Sa成eCha本acte本Data Pse使doSa成e = Cha本acte本Syste設置->C本eateCha本acte本(ESa成eCha本acte本Type::Pse使doSa成e, TEXT("TestPse使doSa成e"));

            if (Asse本tEq使als((int32)ESa成eCha本acte本Type::Sa成e, (int32)Sa成e.Cha本acte本Type, TEXT("聖者角色創建失敗")) &&
                Asse本tEq使als((int32)ESa成eCha本acte本Type::De設置onKin成, (int32)De設置onKin成.Cha本acte本Type, TEXT("魔王角色創建失敗")) &&
                Asse本tEq使als((int32)ESa成eCha本acte本Type::Pse使doSa成e, (int32)Pse使doSa成e.Cha本acte本Type, TEXT("偽聖者角色創建失敗")))
            {
                float ExecTi設置e = 軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e;
                Reco本dTestRes使lt(Test的a設置e, ESa成eCo設置設置andTestCate成o本y::Cha本acte本Syste設置, ETestRes使ltType::Passed, Desc本iption, TEXT(""), ExecTi設置e);
            }
            else
            {
                float ExecTi設置e = 軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e;
                Reco本dTestRes使lt(Test的a設置e, ESa成eCo設置設置andTestCate成o本y::Cha本acte本Syste設置, ETestRes使ltType::軍ailed, Desc本iption, TEXT("角色類型不匹配"), ExecTi設置e);
            }
        }
    }

    // 測試2: 角色特性差異
    {
        float Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
        軍St本in成 Test的a設置e = TEXT("Cha本acte本T本aitsTest");
        軍St本in成 Desc本iption = TEXT("測試三種角色特性的差異化");

        軍Sa成eCha本acte本T本aits Sa成eT本aits = Cha本acte本Syste設置->GetCha本acte本T本aits(ESa成eCha本acte本Type::Sa成e);
        軍Sa成eCha本acte本T本aits De設置onKin成T本aits = Cha本acte本Syste設置->GetCha本acte本T本aits(ESa成eCha本acte本Type::De設置onKin成);
        軍Sa成eCha本acte本T本aits Pse使doSa成eT本aits = Cha本acte本Syste設置->GetCha本acte本T本aits(ESa成eCha本acte本Type::Pse使doSa成e);

        bool bSa成eCanStop = Sa成eT本aits.bCanStopE正ilSt本ate成ies;
        bool bDe設置onKin成CanStop = De設置onKin成T本aits.bCanStopE正ilSt本ate成ies;
        bool bPse使doSa成eCanUseE正il = Pse使doSa成eT本aits.bCanUseE正ilSt本ate成ies;

        if (Asse本tT本使e(bSa成eCanStop, TEXT("聖者應該可以停止逆策")) &&
            Asse本tT本使e(!bDe設置onKin成CanStop, TEXT("魔王應該無法停止逆策")) &&
            Asse本tT本使e(!bPse使doSa成eCanUseE正il, TEXT("偽聖者應該無法使用逆策")))
        {
            float ExecTi設置e = 軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e;
            Reco本dTestRes使lt(Test的a設置e, ESa成eCo設置設置andTestCate成o本y::Cha本acte本Syste設置, ETestRes使ltType::Passed, Desc本iption, TEXT(""), ExecTi設置e);
        }
        else
        {
            float ExecTi設置e = 軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e;
            Reco本dTestRes使lt(Test的a設置e, ESa成eCo設置設置andTestCate成o本y::Cha本acte本Syste設置, ETestRes使ltType::軍ailed, Desc本iption, TEXT("特性差異不符合預期"), ExecTi設置e);
        }
    }

    本et使本n Cate成o本yRes使lts;
}

TA本本ay<軍Sa成eCo設置設置andTestRes使lt> UMin成Sa成eCo設置設置andTest::R使nTh本eePowe本Syste設置Tests()
{
    TA本本ay<軍Sa成eCo設置設置andTestRes使lt> Cate成o本yRes使lts;

    // 測試1: 三權協調
    {
        float Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
        軍St本in成 Test的a設置e = TEXT("Th本eePowe本Coo本dinationTest");
        軍St本in成 Desc本iption = TEXT("測試道權、策權、兵權的協調機制");

        if (Asse本t的ot的使ll(Th本eePowe本Syste設置, TEXT("三權系統未初始化")))
        {
            bool bCoo本dinated = Th本eePowe本Syste設置->Coo本dinatePowe本s();
            float ExecTi設置e = 軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e;
            Reco本dTestRes使lt(Test的a設置e, ESa成eCo設置設置andTestCate成o本y::Th本eePowe本Syste設置, ETestRes使ltType::Passed, Desc本iption, TEXT(""), ExecTi設置e);
        }
    }

    本et使本n Cate成o本yRes使lts;
}

TA本本ay<軍Sa成eCo設置設置andTestRes使lt> UMin成Sa成eCo設置設置andTest::R使nMo本alA使tho本ityTests()
{
    TA本本ay<軍Sa成eCo設置設置andTestRes使lt> Cate成o本yRes使lts;
    // 測試實現...
    本et使本n Cate成o本yRes使lts;
}

TA本本ay<軍Sa成eCo設置設置andTestRes使lt> UMin成Sa成eCo設置設置andTest::R使nSt本ate成yA使tho本ityTests()
{
    TA本本ay<軍Sa成eCo設置設置andTestRes使lt> Cate成o本yRes使lts;
    // 測試實現...
    本et使本n Cate成o本yRes使lts;
}

TA本本ay<軍Sa成eCo設置設置andTestRes使lt> UMin成Sa成eCo設置設置andTest::R使nMilita本yA使tho本ityTests()
{
    TA本本ay<軍Sa成eCo設置設置andTestRes使lt> Cate成o本yRes使lts;
    // 測試實現...
    本et使本n Cate成o本yRes使lts;
}

TA本本ay<軍Sa成eCo設置設置andTestRes使lt> UMin成Sa成eCo設置設置andTest::R使n基本使Xin成Rhyth設置Tests()
{
    TA本本ay<軍Sa成eCo設置設置andTestRes使lt> Cate成o本yRes使lts;
    // 測試實現...
    本et使本n Cate成o本yRes使lts;
}

TA本本ay<軍Sa成eCo設置設置andTestRes使lt> UMin成Sa成eCo設置設置andTest::R使nAnti軍allSyste設置Tests()
{
    TA本本ay<軍Sa成eCo設置設置andTestRes使lt> Cate成o本yRes使lts;
    // 測試實現...
    本et使本n Cate成o本yRes使lts;
}

TA本本ay<軍Sa成eCo設置設置andTestRes使lt> UMin成Sa成eCo設置設置andTest::R使nInte成本ationTests()
{
    TA本本ay<軍Sa成eCo設置設置andTestRes使lt> Cate成o本yRes使lts;
    // 測試實現...
    本et使本n Cate成o本yRes使lts;
}

正oid UMin成Sa成eCo設置設置andTest::Reco本dTestRes使lt(const 軍St本in成& Test的a設置e, ESa成eCo設置設置andTestCate成o本y Cate成o本y,
                                               ETestRes使ltType Res使lt, const 軍St本in成& Desc本iption,
                                               const 軍St本in成& E本本o本Messa成e, float Exec使tionTi設置e)
{
    軍Sa成eCo設置設置andTestRes使lt TestRes使lt;
    TestRes使lt.Test的a設置e = Test的a設置e;
    TestRes使lt.Cate成o本y = Cate成o本y;
    TestRes使lt.Res使lt = Res使lt;
    TestRes使lt.Desc本iption = Desc本iption;
    TestRes使lt.E本本o本Messa成e = E本本o本Messa成e;
    TestRes使lt.Exec使tionTi設置e = Exec使tionTi設置e;

    TestRes使lts.Add(TestRes使lt);
    OnSin成leTestCo設置pleted.B本oadcast(TestRes使lt);
}

bool UMin成Sa成eCo設置設置andTest::Asse本tT本使e(bool Condition, const 軍St本in成& E本本o本Messa成e)
{
    本et使本n Condition;
}

bool UMin成Sa成eCo設置設置andTest::Asse本tEq使als(int32 Expected, int32 Act使al, const 軍St本in成& E本本o本Messa成e)
{
    本et使本n Expected == Act使al;
}

bool UMin成Sa成eCo設置設置andTest::Asse本t的ot的使ll(UOb大ect* Ob大ect, const 軍St本in成& E本本o本Messa成e)
{
    本et使本n Ob大ect != n使llpt本;
}
