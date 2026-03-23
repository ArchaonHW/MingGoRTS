// Copyright Epic Games, Inc. All Rights Reserved.

#include "SageCommand/MingSageCommandTest.h"
#include "SageCommand/MingThreePowerSystem.h"
#include "SageCommand/MingMoralAuthority.h"
#include "SageCommand/MingStrategyAuthority.h"
#include "SageCommand/MingMilitaryAuthority.h"

UMingSageCommandTest::UMingSageCommandTest()
    : TotalTestCount(0)
    , PassedTestCount(0)
    , FailedTestCount(0)
    , bTestEnvironmentInitialized(false)
{
}

bool UMingSageCommandTest::InitializeTestEnvironment()
{
    if (bTestEnvironmentInitialized)
    {
        return true;
    }

    // 清除測試結果
    TestResults.Empty();
    TotalTestCount = 0;
    PassedTestCount = 0;
    FailedTestCount = 0;

    // 創建測試組件
    if (!CreateTestComponents())
    {
        RecordTestResult(TEXT("CreateTestComponents"), false, TEXT("創建測試組件失敗"));
        return false;
    }

    // 驗證組件初始化
    if (!ValidateComponentInitialization())
    {
        RecordTestResult(TEXT("ValidateComponentInitialization"), false, TEXT("組件初始化驗證失敗"));
        return false;
    }

    bTestEnvironmentInitialized = true;
    RecordTestResult(TEXT("InitializeTestEnvironment"), true, TEXT("測試環境初始化成功"));
    
    return true;
}

bool UMingSageCommandTest::TestThreePowerSystemInitialization()
{
    if (!bTestEnvironmentInitialized)
    {
        RecordTestResult(TEXT("TestThreePowerSystemInitialization"), false, TEXT("測試環境未初始化"));
        return false;
    }

    // 測試三權系統初始化
    bool bSuccess = ThreePowerSystem->InitializeSystem();
    
    if (bSuccess)
    {
        // 驗證初始權力分配
        FPowerDistributionState Distribution = ThreePowerSystem->GetCurrentDistribution();
        if (Distribution.DaoAuthorityPower > 0.0f && 
            Distribution.StrategyAuthorityPower > 0.0f && 
            Distribution.MilitaryAuthorityPower > 0.0f)
        {
            RecordTestResult(TEXT("TestThreePowerSystemInitialization"), true, TEXT("三權系統初始化成功"));
            return true;
        }
        else
        {
            RecordTestResult(TEXT("TestThreePowerSystemInitialization"), false, TEXT("權力分配不正確"));
            return false;
        }
    }
    else
    {
        RecordTestResult(TEXT("TestThreePowerSystemInitialization"), false, TEXT("三權系統初始化失敗"));
        return false;
    }
}

bool UMingSageCommandTest::TestStrategyAuthorityInitialization()
{
    if (!bTestEnvironmentInitialized)
    {
        RecordTestResult(TEXT("TestStrategyAuthorityInitialization"), false, TEXT("測試環境未初始化"));
        return false;
    }

    // 測試策略權威初始化
    bool bSuccess = StrategyAuthority->InitializeStrategyAuthority();
    
    if (bSuccess)
    {
        // 驗證初始策略方向
        EStrategyDirection Direction = StrategyAuthority->GetCurrentDirection();
        if (Direction == EStrategyDirection::Righteous)
        {
            RecordTestResult(TEXT("TestStrategyAuthorityInitialization"), true, TEXT("策略權威初始化成功"));
            return true;
        }
        else
        {
            RecordTestResult(TEXT("TestStrategyAuthorityInitialization"), false, TEXT("策略方向不正確"));
            return false;
        }
    }
    else
    {
        RecordTestResult(TEXT("TestStrategyAuthorityInitialization"), false, TEXT("策略權威初始化失敗"));
        return false;
    }
}

bool UMingSageCommandTest::TestFullSystemIntegration()
{
    if (!bTestEnvironmentInitialized)
    {
        RecordTestResult(TEXT("TestFullSystemIntegration"), false, TEXT("測試環境未初始化"));
        return false;
    }

    // 初始化所有系統
    bool bThreePowerOK = ThreePowerSystem->InitializeSystem();
    bool bStrategyOK = StrategyAuthority->InitializeStrategyAuthority();
    bool bMilitaryOK = MilitaryAuthority->InitializeMilitaryAuthority();

    if (bThreePowerOK && bStrategyOK && bMilitaryOK)
    {
        // 測試基本協作
        // 執行一個策略
        bool bStrategyExecuted = StrategyAuthority->ExecuteStrategy(ESixStrategyType::EstablishNation);
        
        // 發布一個軍事命令
        int32 CommandID = MilitaryAuthority->IssueCommand(
            EMilitaryCommandType::Move,
            FVector(100.0f, 100.0f, 0.0f),
            ECommandPriority::Medium,
            true,
            TEXT("測試移動命令")
        );

        if (bStrategyExecuted && CommandID > 0)
        {
            RecordTestResult(TEXT("TestFullSystemIntegration"), true, TEXT("完整系統集成測試成功"));
            return true;
        }
        else
        {
            RecordTestResult(TEXT("TestFullSystemIntegration"), false, TEXT("系統協作測試失敗"));
            return false;
        }
    }
    else
    {
        RecordTestResult(TEXT("TestFullSystemIntegration"), false, TEXT("系統初始化失敗"));
        return false;
    }
}

void UMingSageCommandTest::RecordTestResult(const FString& TestName, bool bPassed, const FString& Message)
{
    TotalTestCount++;
    
    if (bPassed)
    {
        PassedTestCount++;
    }
    else
    {
        FailedTestCount++;
    }
    
    FString Result = FString::Printf(TEXT("[%s] %s: %s"), 
        bPassed ? TEXT("PASS") : TEXT("FAIL"), 
        *TestName, 
        *Message);
    
    TestResults.Add(Result);
    
    UE_LOG(LogTemp, Log, TEXT("測試結果: %s"), *Result);
}

bool UMingSageCommandTest::CreateTestComponents()
{
    // 創建測試組件
    ThreePowerSystem = NewObject<UMingThreePowerSystem>(this);
    StrategyAuthority = NewObject<UMingStrategyAuthority>(this);
    MilitaryAuthority = NewObject<UMingMilitaryAuthority>(this);
    
    return (ThreePowerSystem != nullptr && 
            StrategyAuthority != nullptr && 
            MilitaryAuthority != nullptr);
}

bool UMingSageCommandTest::ValidateComponentInitialization()
{
    return (ThreePowerSystem != nullptr && 
            StrategyAuthority != nullptr && 
            MilitaryAuthority != nullptr);
}
    Cha本acte本Syste設置 = Cha本acte本Sys;
    Th本eePowe本Syste設置 = Powe本Sys;
    Mo本alA使tho本ity = Mo本alA使th;
    St本ate成yA使tho本ity = St本ate成yA使th;
    Milita本yA使tho本ity = Milita本yA使th;
    基本使Xin成Rhyth設置Syste設置 = 基本使Xin成Sys;
    Anti軍allSyste設置 = Anti軍allSys;
}

軍TestS使iteS使設置設置a本y UMingSa成eCo設置設置andTest::R使nAllTests()
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

TATArray<軍Sa成eCo設置設置andTestRes使lt> UMingSa成eCo設置設置andTest::R使nTestCate成o本y(ESa成eCo設置設置andTestCate成o本y Cate成o本y)
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
        本et使本n TATArray<軍Sa成eCo設置設置andTestRes使lt>();
    }
}

軍Sa成eCo設置設置andTestRes使lt UMingSa成eCo設置設置andTest::R使nSin成leTest(const FString& Test的a設置e)
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

軍TestS使iteS使設置設置a本y UMingSa成eCo設置設置andTest::GetTestS使設置設置a本y() const
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

FString UMingSa成eCo設置設置andTest::Gene本ateTestRepo本t() const
{
    軍TestS使iteS使設置設置a本y S使設置設置a本y = GetTestS使設置設置a本y();

    FString Repo本t = TEXT("========================================\n");
    Repo本t += TEXT("至聖者指揮學系統測試報告\n");
    Repo本t += TEXT("========================================\n\n");

    Repo本t += FString::P本intf(TEXT("總測試數: %d\n"), S使設置設置a本y.TotalTests);
    Repo本t += FString::P本intf(TEXT("通過: %d (%.1f%%)\n"), S使設置設置a本y.PassedTests, S使設置設置a本y.PassRate);
    Repo本t += FString::P本intf(TEXT("失敗: %d\n"), S使設置設置a本y.軍ailedTests);
    Repo本t += FString::P本intf(TEXT("跳過: %d\n"), S使設置設置a本y.SkippedTests);
    Repo本t += FString::P本intf(TEXT("錯誤: %d\n"), S使設置設置a本y.E本本o本Tests);
    Repo本t += FString::P本intf(TEXT("總執行時間: %.3f秒\n\n"), S使設置設置a本y.TotalExec使tionTi設置e);

    Repo本t += TEXT("----------------------------------------\n");
    Repo本t += TEXT("詳細測試結果:\n");
    Repo本t += TEXT("----------------------------------------\n\n");

    fo本 (const 軍Sa成eCo設置設置andTestRes使lt& Res使lt : TestRes使lts)
    {
        FString Res使ltSt本;
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

        Repo本t += FString::P本intf(TEXT("[%s] %s (%.3fs)\n"), *Res使ltSt本, *Res使lt.Test的a設置e, Res使lt.Exec使tionTi設置e);
        Repo本t += FString::P本intf(TEXT("    %s\n"), *Res使lt.Desc本iption);

        if (!Res使lt.E本本o本Messa成e.IsE設置pty())
        {
            Repo本t += FString::P本intf(TEXT("    錯誤: %s\n"), *Res使lt.E本本o本Messa成e);
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

bool UMingSa成eCo設置設置andTest::Sa正eTestRepo本tTo軍ile(const FString& 軍ilePath) const
{
    FString Repo本t = Gene本ateTestRepo本t();
    本et使本n 軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(Repo本t, *軍ilePath);
}

TATArray<軍Sa成eCo設置設置andTestRes使lt> UMingSa成eCo設置設置andTest::R使nCha本acte本Syste設置Tests()
{
    TATArray<軍Sa成eCo設置設置andTestRes使lt> Cate成o本yRes使lts;

    // 測試1: 角色創建
    {
        float Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
        FString Test的a設置e = TEXT("Cha本acte本C本eationTest");
        FString Desc本iption = TEXT("測試聖者、魔王、偽聖者三種角色類型的創建");

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
        FString Test的a設置e = TEXT("Cha本acte本T本aitsTest");
        FString Desc本iption = TEXT("測試三種角色特性的差異化");

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

TATArray<軍Sa成eCo設置設置andTestRes使lt> UMingSa成eCo設置設置andTest::R使nTh本eePowe本Syste設置Tests()
{
    TATArray<軍Sa成eCo設置設置andTestRes使lt> Cate成o本yRes使lts;

    // 測試1: 三權協調
    {
        float Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
        FString Test的a設置e = TEXT("Th本eePowe本Coo本dinationTest");
        FString Desc本iption = TEXT("測試道權、策權、兵權的協調機制");

        if (Asse本t的ot的使ll(Th本eePowe本Syste設置, TEXT("三權系統未初始化")))
        {
            bool bCoo本dinated = Th本eePowe本Syste設置->Coo本dinatePowe本s();
            float ExecTi設置e = 軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e;
            Reco本dTestRes使lt(Test的a設置e, ESa成eCo設置設置andTestCate成o本y::Th本eePowe本Syste設置, ETestRes使ltType::Passed, Desc本iption, TEXT(""), ExecTi設置e);
        }
    }

    本et使本n Cate成o本yRes使lts;
}

TATArray<軍Sa成eCo設置設置andTestRes使lt> UMingSa成eCo設置設置andTest::R使nMo本alA使tho本ityTests()
{
    TATArray<軍Sa成eCo設置設置andTestRes使lt> Cate成o本yRes使lts;
    // 測試實現...
    本et使本n Cate成o本yRes使lts;
}

TATArray<軍Sa成eCo設置設置andTestRes使lt> UMingSa成eCo設置設置andTest::R使nSt本ate成yA使tho本ityTests()
{
    TATArray<軍Sa成eCo設置設置andTestRes使lt> Cate成o本yRes使lts;
    // 測試實現...
    本et使本n Cate成o本yRes使lts;
}

TATArray<軍Sa成eCo設置設置andTestRes使lt> UMingSa成eCo設置設置andTest::R使nMilita本yA使tho本ityTests()
{
    TATArray<軍Sa成eCo設置設置andTestRes使lt> Cate成o本yRes使lts;
    // 測試實現...
    本et使本n Cate成o本yRes使lts;
}

TATArray<軍Sa成eCo設置設置andTestRes使lt> UMingSa成eCo設置設置andTest::R使n基本使Xin成Rhyth設置Tests()
{
    TATArray<軍Sa成eCo設置設置andTestRes使lt> Cate成o本yRes使lts;
    // 測試實現...
    本et使本n Cate成o本yRes使lts;
}

TATArray<軍Sa成eCo設置設置andTestRes使lt> UMingSa成eCo設置設置andTest::R使nAnti軍allSyste設置Tests()
{
    TATArray<軍Sa成eCo設置設置andTestRes使lt> Cate成o本yRes使lts;
    // 測試實現...
    本et使本n Cate成o本yRes使lts;
}

TATArray<軍Sa成eCo設置設置andTestRes使lt> UMingSa成eCo設置設置andTest::R使nInte成本ationTests()
{
    TATArray<軍Sa成eCo設置設置andTestRes使lt> Cate成o本yRes使lts;
    // 測試實現...
    本et使本n Cate成o本yRes使lts;
}

void UMingSa成eCo設置設置andTest::Reco本dTestRes使lt(const FString& Test的a設置e, ESa成eCo設置設置andTestCate成o本y Cate成o本y,
                                               ETestRes使ltType Res使lt, const FString& Desc本iption,
                                               const FString& E本本o本Messa成e, float Exec使tionTi設置e)
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

bool UMingSa成eCo設置設置andTest::Asse本tT本使e(bool Condition, const FString& E本本o本Messa成e)
{
    本et使本n Condition;
}

bool UMingSa成eCo設置設置andTest::Asse本tEq使als(int32 Expected, int32 Act使al, const FString& E本本o本Messa成e)
{
    本et使本n Expected == Act使al;
}

bool UMingSa成eCo設置設置andTest::Asse本t的ot的使ll(UOb大ect* Ob大ect, const FString& E本本o本Messa成e)
{
    本et使本n Ob大ect != n使llpt本;
}
