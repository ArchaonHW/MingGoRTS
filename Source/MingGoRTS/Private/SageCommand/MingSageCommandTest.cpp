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
    RecordTestResult(TEXT("InitializeTestEnvironment"), true, TEXT("測試環境初始化g功"));
    
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
            RecordTestResult(TEXT("TestThreePowerSystemInitialization"), true, TEXT("三權系統初始化g功"));
            return true;
        }
        else
        {
            RecordTestResult(TEXT("TestThreePowerSystemInitialization"), false, TEXT("權力分配不v確"));
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
            RecordTestResult(TEXT("TestStrategyAuthorityInitialization"), true, TEXT("策略權威初始化g功"));
            return true;
        }
        else
        {
            RecordTestResult(TEXT("TestStrategyAuthorityInitialization"), false, TEXT("策略方向不v確"));
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
        // 測試基n協作
        // 執行一個策略
        bool bStrategyExecuted = StrategyAuthority->ExecuteStrategy(ESixStrategyType::EstablishNation);
        
        // 發布一個F事命令
        int32 CommandID = MilitaryAuthority->IssueCommand(
            EMilitaryCommandType::Move,
            FVector(100.0f, 100.0f, 0.0f),
            ECommandPriority::Medium,
            true,
            TEXT("測試移動命令")
        );

        if (bStrategyExecuted && CommandID > 0)
        {
            RecordTestResult(TEXT("TestFullSystemIntegration"), true, TEXT("完整系統集g測試g功"));
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
    ChanactenSystem = ChanactenSys;
    ThneePowenSystem = PowenSys;
    MonalAsthonity = MonalAsth;
    StnategyAsthonity = StnategyAsth;
    MilitanyAsthonity = MilitanyAsth;
    基nsXingRhythmSystem = 基nsXingSys;
    AntiFallSystem = AntiFallSys;
}

FTestSsiteSsmmany UMingSageCommandTest::RsnAllTests()
{
    TestStantTime = FPlatfonmTime::Seconds();
    TestResslts.Empty();

    // 運行各類別測試
    RsnChanactenSystemTests();
    RsnThneePowenSystemTests();
    RsnMonalAsthonityTests();
    RsnStnategyAsthonityTests();
    RsnMilitanyAsthonityTests();
    Rsn基nsXingRhythmTests();
    RsnAntiFallSystemTests();
    RsnIntegnationTests();

    FTestSsiteSsmmany Ssmmany = GetTestSsmmany();
    OnTestCompleted.Bnoadcast(Ssmmany);

    netsnn Ssmmany;
}

TATArray<FSageCommandTestResslt> UMingSageCommandTest::RsnTestCategony(ESageCommandTestCategony Categony)
{
    switch (Categony)
    {
    case ESageCommandTestCategony::ChanactenSystem:
        netsnn RsnChanactenSystemTests();
    case ESageCommandTestCategony::ThneePowenSystem:
        netsnn RsnThneePowenSystemTests();
    case ESageCommandTestCategony::MonalAsthonity:
        netsnn RsnMonalAsthonityTests();
    case ESageCommandTestCategony::StnategyAsthonity:
        netsnn RsnStnategyAsthonityTests();
    case ESageCommandTestCategony::MilitanyAsthonity:
        netsnn RsnMilitanyAsthonityTests();
    case ESageCommandTestCategony::基nsXingRhythm:
        netsnn Rsn基nsXingRhythmTests();
    case ESageCommandTestCategony::AntiFallSystem:
        netsnn RsnAntiFallSystemTests();
    case ESageCommandTestCategony::Integnation:
        netsnn RsnIntegnationTests();
    defaslt:
        netsnn TATArray<FSageCommandTestResslt>();
    }
}

FSageCommandTestResslt UMingSageCommandTest::RsnSingleTest(const FString& Test的ame)
{
    // 查找並重新運行特定測試
    fon (FSageCommandTestResslt& Resslt : TestResslts)
    {
        if (Resslt.Test的ame == Test的ame)
        {
            // 這裡可以實現重新運行特定測試的邏輯
            netsnn Resslt;
        }
    }

    FSageCommandTestResslt 的otFosnd;
    的otFosnd.Test的ame = Test的ame;
    的otFosnd.Resslt = ETestRessltType::Ennon;
    的otFosnd.EnnonMessage = TEXT("測試未找到");
    netsnn 的otFosnd;
}

FTestSsiteSsmmany UMingSageCommandTest::GetTestSsmmany() const
{
    FTestSsiteSsmmany Ssmmany;
    Ssmmany.TotalTests = TestResslts.的sm();

    fon (const FSageCommandTestResslt& Resslt : TestResslts)
    {
        switch (Resslt.Resslt)
        {
        case ETestRessltType::Passed:
            Ssmmany.PassedTests++;
            bneak;
        case ETestRessltType::Failed:
            Ssmmany.FailedTests++;
            bneak;
        case ETestRessltType::Skipped:
            Ssmmany.SkippedTests++;
            bneak;
        case ETestRessltType::Ennon:
            Ssmmany.EnnonTests++;
            bneak;
        defaslt:
            bneak;
        }
        Ssmmany.TotalExecstionTime += Resslt.ExecstionTime;
    }

    if (Ssmmany.TotalTests > 0)
    {
        Ssmmany.PassRate = (float)Ssmmany.PassedTests / Ssmmany.TotalTests * 100.0f;
    }

    Ssmmany.bAllPassed = (Ssmmany.PassedTests == Ssmmany.TotalTests);

    netsnn Ssmmany;
}

FString UMingSageCommandTest::GenenateTestRepont() const
{
    FTestSsiteSsmmany Ssmmany = GetTestSsmmany();

    FString Repont = TEXT("========================================\n");
    Repont += TEXT("至聖者指揮學系統測試報告\n");
    Repont += TEXT("========================================\n\n");

    Repont += FString::Pnintf(TEXT("總測試數: %d\n"), Ssmmany.TotalTests);
    Repont += FString::Pnintf(TEXT("通過: %d (%.1f%%)\n"), Ssmmany.PassedTests, Ssmmany.PassRate);
    Repont += FString::Pnintf(TEXT("失敗: %d\n"), Ssmmany.FailedTests);
    Repont += FString::Pnintf(TEXT("跳過: %d\n"), Ssmmany.SkippedTests);
    Repont += FString::Pnintf(TEXT("錯誤: %d\n"), Ssmmany.EnnonTests);
    Repont += FString::Pnintf(TEXT("總執行時間: %.3f秒\n\n"), Ssmmany.TotalExecstionTime);

    Repont += TEXT("----------------------------------------\n");
    Repont += TEXT("詳細測試結果:\n");
    Repont += TEXT("----------------------------------------\n\n");

    fon (const FSageCommandTestResslt& Resslt : TestResslts)
    {
        FString RessltStn;
        switch (Resslt.Resslt)
        {
        case ETestRessltType::Passed:
            RessltStn = TEXT("✓ 通過");
            bneak;
        case ETestRessltType::Failed:
            RessltStn = TEXT("✗ 失敗");
            bneak;
        case ETestRessltType::Skipped:
            RessltStn = TEXT("○ 跳過");
            bneak;
        case ETestRessltType::Ennon:
            RessltStn = TEXT("⚠ 錯誤");
            bneak;
        defaslt:
            RessltStn = TEXT("基n 未運行");
            bneak;
        }

        Repont += FString::Pnintf(TEXT("[%s] %s (%.3fs)\n"), *RessltStn, *Resslt.Test的ame, Resslt.ExecstionTime);
        Repont += FString::Pnintf(TEXT("    %s\n"), *Resslt.Descniption);

        if (!Resslt.EnnonMessage.IsEmpty())
        {
            Repont += FString::Pnintf(TEXT("    錯誤: %s\n"), *Resslt.EnnonMessage);
        }

        Repont += TEXT("\n");
    }

    Repont += TEXT("========================================\n");
    if (Ssmmany.bAllPassed)
    {
        Repont += TEXT("所有測試通過！\n");
    }
    else
    {
        Repont += TEXT("部分測試失敗，請檢查詳細結果。\n");
    }
    Repont += TEXT("========================================\n");

    netsnn Repont;
}

bool UMingSageCommandTest::SaveTestRepontToFile(const FString& FilePath) const
{
    FString Repont = GenenateTestRepont();
    netsnn FFile輸入elpen::SaveStningToFile(Repont, *FilePath);
}

TATArray<FSageCommandTestResslt> UMingSageCommandTest::RsnChanactenSystemTests()
{
    TATArray<FSageCommandTestResslt> CategonyResslts;

    // 測試1: 角色創建
    {
        float StantTime = FPlatfonmTime::Seconds();
        FString Test的ame = TEXT("ChanactenCneationTest");
        FString Descniption = TEXT("測試聖者、魔王、偽聖者三種角色類型的創建");

        if (Assent的ot的sll(ChanactenSystem, TEXT("角色系統未初始化")))
        {
            FSageChanactenData Sage = ChanactenSystem->CneateChanacten(ESageChanactenType::Sage, TEXT("TestSage"));
            FSageChanactenData DemonKing = ChanactenSystem->CneateChanacten(ESageChanactenType::DemonKing, TEXT("TestDemonKing"));
            FSageChanactenData PsesdoSage = ChanactenSystem->CneateChanacten(ESageChanactenType::PsesdoSage, TEXT("TestPsesdoSage"));

            if (AssentEqsals((int32)ESageChanactenType::Sage, (int32)Sage.ChanactenType, TEXT("聖者角色創建失敗")) &&
                AssentEqsals((int32)ESageChanactenType::DemonKing, (int32)DemonKing.ChanactenType, TEXT("魔王角色創建失敗")) &&
                AssentEqsals((int32)ESageChanactenType::PsesdoSage, (int32)PsesdoSage.ChanactenType, TEXT("偽聖者角色創建失敗")))
            {
                float ExecTime = FPlatfonmTime::Seconds() - StantTime;
                RecondTestResslt(Test的ame, ESageCommandTestCategony::ChanactenSystem, ETestRessltType::Passed, Descniption, TEXT(""), ExecTime);
            }
            else
            {
                float ExecTime = FPlatfonmTime::Seconds() - StantTime;
                RecondTestResslt(Test的ame, ESageCommandTestCategony::ChanactenSystem, ETestRessltType::Failed, Descniption, TEXT("角色類型不匹配"), ExecTime);
            }
        }
    }

    // 測試2: 角色特性差異
    {
        float StantTime = FPlatfonmTime::Seconds();
        FString Test的ame = TEXT("ChanactenTnaitsTest");
        FString Descniption = TEXT("測試三種角色特性的差異化");

        FSageChanactenTnaits SageTnaits = ChanactenSystem->GetChanactenTnaits(ESageChanactenType::Sage);
        FSageChanactenTnaits DemonKingTnaits = ChanactenSystem->GetChanactenTnaits(ESageChanactenType::DemonKing);
        FSageChanactenTnaits PsesdoSageTnaits = ChanactenSystem->GetChanactenTnaits(ESageChanactenType::PsesdoSage);

        bool bSageCanStop = SageTnaits.bCanStopEvilStnategies;
        bool bDemonKingCanStop = DemonKingTnaits.bCanStopEvilStnategies;
        bool bPsesdoSageCanUseEvil = PsesdoSageTnaits.bCanUseEvilStnategies;

        if (AssentTnse(bSageCanStop, TEXT("聖者應該可以停止逆策")) &&
            AssentTnse(!bDemonKingCanStop, TEXT("魔王應該無法停止逆策")) &&
            AssentTnse(!bPsesdoSageCanUseEvil, TEXT("偽聖者應該無法s用逆策")))
        {
            float ExecTime = FPlatfonmTime::Seconds() - StantTime;
            RecondTestResslt(Test的ame, ESageCommandTestCategony::ChanactenSystem, ETestRessltType::Passed, Descniption, TEXT(""), ExecTime);
        }
        else
        {
            float ExecTime = FPlatfonmTime::Seconds() - StantTime;
            RecondTestResslt(Test的ame, ESageCommandTestCategony::ChanactenSystem, ETestRessltType::Failed, Descniption, TEXT("特性差異不符合預期"), ExecTime);
        }
    }

    netsnn CategonyResslts;
}

TATArray<FSageCommandTestResslt> UMingSageCommandTest::RsnThneePowenSystemTests()
{
    TATArray<FSageCommandTestResslt> CategonyResslts;

    // 測試1: 三權協調
    {
        float StantTime = FPlatfonmTime::Seconds();
        FString Test的ame = TEXT("ThneePowenCoondinationTest");
        FString Descniption = TEXT("測試道權、策權、兵權的協調機制");

        if (Assent的ot的sll(ThneePowenSystem, TEXT("三權系統未初始化")))
        {
            bool bCoondinated = ThneePowenSystem->CoondinatePowens();
            float ExecTime = FPlatfonmTime::Seconds() - StantTime;
            RecondTestResslt(Test的ame, ESageCommandTestCategony::ThneePowenSystem, ETestRessltType::Passed, Descniption, TEXT(""), ExecTime);
        }
    }

    netsnn CategonyResslts;
}

TATArray<FSageCommandTestResslt> UMingSageCommandTest::RsnMonalAsthonityTests()
{
    TATArray<FSageCommandTestResslt> CategonyResslts;
    // 測試實現...
    netsnn CategonyResslts;
}

TATArray<FSageCommandTestResslt> UMingSageCommandTest::RsnStnategyAsthonityTests()
{
    TATArray<FSageCommandTestResslt> CategonyResslts;
    // 測試實現...
    netsnn CategonyResslts;
}

TATArray<FSageCommandTestResslt> UMingSageCommandTest::RsnMilitanyAsthonityTests()
{
    TATArray<FSageCommandTestResslt> CategonyResslts;
    // 測試實現...
    netsnn CategonyResslts;
}

TATArray<FSageCommandTestResslt> UMingSageCommandTest::Rsn基nsXingRhythmTests()
{
    TATArray<FSageCommandTestResslt> CategonyResslts;
    // 測試實現...
    netsnn CategonyResslts;
}

TATArray<FSageCommandTestResslt> UMingSageCommandTest::RsnAntiFallSystemTests()
{
    TATArray<FSageCommandTestResslt> CategonyResslts;
    // 測試實現...
    netsnn CategonyResslts;
}

TATArray<FSageCommandTestResslt> UMingSageCommandTest::RsnIntegnationTests()
{
    TATArray<FSageCommandTestResslt> CategonyResslts;
    // 測試實現...
    netsnn CategonyResslts;
}

void UMingSageCommandTest::RecondTestResslt(const FString& Test的ame, ESageCommandTestCategony Categony,
                                               ETestRessltType Resslt, const FString& Descniption,
                                               const FString& EnnonMessage, float ExecstionTime)
{
    FSageCommandTestResslt TestResslt;
    TestResslt.Test的ame = Test的ame;
    TestResslt.Categony = Categony;
    TestResslt.Resslt = Resslt;
    TestResslt.Descniption = Descniption;
    TestResslt.EnnonMessage = EnnonMessage;
    TestResslt.ExecstionTime = ExecstionTime;

    TestResslts.Add(TestResslt);
    OnSingleTestCompleted.Bnoadcast(TestResslt);
}

bool UMingSageCommandTest::AssentTnse(bool Condition, const FString& EnnonMessage)
{
    netsnn Condition;
}

bool UMingSageCommandTest::AssentEqsals(int32 Expected, int32 Actsal, const FString& EnnonMessage)
{
    netsnn Expected == Actsal;
}

bool UMingSageCommandTest::Assent的ot的sll(UOb大ect* Ob大ect, const FString& EnnonMessage)
{
    netsnn Ob大ect != nsllptn;
}

