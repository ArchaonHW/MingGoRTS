#include "Factions/MingRTSFactionSystemTest.h"
#include "Factions/MingRTSFactionManager.h"
#include "Misc/ScopeExit.h"

UMingRTSFactionSystemTest::UMingRTSFactionSystemTest()
{
}

void UMingRTSFactionSystemTest::InitializeTestSuite()
{
    TestFactionManager = NewObject<UMingRTSFactionManager>();
    TestFactionManager->InitializeFactionManager();
}

FFactionSystemTestResult UMingRTSFactionSystemTest::CreateTestResult(const FString& TestName, bool bPassed, const FString& ErrorMessage)
{
    FFactionSystemTestResult Result;
    Result.TestName = TestName;
    Result.bPassed = bPassed;
    Result.ErrorMessage = ErrorMessage;
    return Result;
}

void UMingRTSFactionSystemTest::LogTestResult(const FFactionSystemTestResult& Result)
{
    if (Result.bPassed)
    {
        UE_LOG(LogTemp, Log, TEXT("[PASS] %s"), *Result.TestName);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[FAIL] %s: %s"), *Result.TestName, *Result.ErrorMessage);
    }
}

FFactionSystemTestSuiteResult UMingRTSFactionSystemTest::RunAllTests()
{
    FFactionSystemTestSuiteResult SuiteResult;
    double StartTime = FPlatformTime::Seconds();

    // 初始化測試套件
    InitializeTestSuite();

    TArray<FFactionSystemTestResult> Results;

    // 運行所有測試
    Results.Add(TestFactionManagerCreation());
    Results.Add(TestAll12FactionsInitialized());
    Results.Add(TestFactionDataIntegrity());
    Results.Add(TestFactionRelations());
    Results.Add(TestPlayerFactionSetting());
    Results.Add(TestVictoryPointsSystem());
    Results.Add(TestTerritoryControl());
    Results.Add(TestWarDeclaration());
    Results.Add(TestAllianceFormation());
    Results.Add(TestFactionDefeatAndRevival());
    Results.Add(TestUniqueUnits());
    Results.Add(TestSpecialMechanics());
    Results.Add(TestAIConfiguration());
    Results.Add(TestTurnSystem());
    Results.Add(TestBeiyangGovernment());
    Results.Add(TestChineseCommunistParty());
    Results.Add(TestXinjiangFaction());

    // 計算結果
    for (const auto& Result : Results)
    {
        SuiteResult.TestResults.Add(Result);
        SuiteResult.TotalTests++;
        if (Result.bPassed)
        {
            SuiteResult.PassedTests++;
        }
        else
        {
            SuiteResult.FailedTests++;
        }
    }

    double EndTime = FPlatformTime::Seconds();
    SuiteResult.TotalExecutionTime = EndTime - StartTime;

    // 清理
    if (TestFactionManager)
    {
        TestFactionManager->ShutdownFactionManager();
    }

    return SuiteResult;
}

FFactionSystemTestResult UMingRTSFactionSystemTest::TestFactionManagerCreation()
{
    FString TestName = TEXT("FactionManagerCreation");

    UMingRTSFactionManager* Manager = NewObject<UMingRTSFactionManager>();
    if (!Manager)
    {
        return CreateTestResult(TestName, false, TEXT("Failed to create FactionManager"));
    }

    Manager->InitializeFactionManager();
    if (!Manager->IsInitialized())
    {
        return CreateTestResult(TestName, false, TEXT("FactionManager failed to initialize"));
    }

    Manager->ShutdownFactionManager();
    return CreateTestResult(TestName, true);
}

FFactionSystemTestResult UMingRTSFactionSystemTest::TestAll12FactionsInitialized()
{
    FString TestName = TEXT("All12FactionsInitialized");

    if (!TestFactionManager)
    {
        return CreateTestResult(TestName, false, TEXT("TestFactionManager is null"));
    }

    TArray<EFactionType> AllFactions = TestFactionManager->GetAllRegisteredFactions();
    if (AllFactions.Num() != 12)
    {
        return CreateTestResult(TestName, false, FString::Printf(TEXT("Expected 12 factions, got %d"), AllFactions.Num()));
    }

    // 驗證每個勢力都已註冊
    for (uint8 i = 0; i < 12; ++i)
    {
        EFactionType FactionType = static_cast<EFactionType>(i);
        if (!TestFactionManager->IsFactionRegistered(FactionType))
        {
            return CreateTestResult(TestName, false, FString::Printf(TEXT("Faction %d not registered"), i));
        }
    }

    return CreateTestResult(TestName, true);
}

FFactionSystemTestResult UMingRTSFactionSystemTest::TestFactionDataIntegrity()
{
    FString TestName = TEXT("FactionDataIntegrity");

    if (!TestFactionManager)
    {
        return CreateTestResult(TestName, false, TEXT("TestFactionManager is null"));
    }

    // 驗證北洋政府數據完整性
    FFactionData BeiyangData = TestFactionManager->GetFactionData(EFactionType::BeiyangGovernment);
    if (BeiyangData.FactionName.IsEmpty())
    {
        return CreateTestResult(TestName, false, TEXT("BeiyangGovernment name is empty"));
    }

    if (BeiyangData.UniqueUnits.Num() < 2)
    {
        return CreateTestResult(TestName, false, TEXT("BeiyangGovernment should have at least 2 unique units"));
    }

    if (BeiyangData.SpecialMechanics.Num() == 0)
    {
        return CreateTestResult(TestName, false, TEXT("BeiyangGovernment should have special mechanics"));
    }

    return CreateTestResult(TestName, true);
}

FFactionSystemTestResult UMingRTSFactionSystemTest::TestFactionRelations()
{
    FString TestName = TEXT("FactionRelations");

    if (!TestFactionManager)
    {
        return CreateTestResult(TestName, false, TEXT("TestFactionManager is null"));
    }

    // 測試北洋和國民黨的關係（應該是敵對）
    EFactionRelationType Relation = TestFactionManager->GetFactionRelation(
        EFactionType::BeiyangGovernment, EFactionType::NationalistGovernment);

    if (Relation != EFactionRelationType::Hostile && Relation != EFactionRelationType::War)
    {
        return CreateTestResult(TestName, false, TEXT("Beiyang-Nationalist relation should be hostile or war"));
    }

    // 測試修改關係值
    TestFactionManager->ModifyFactionRelationValue(
        EFactionType::BeiyangGovernment, EFactionType::NationalistGovernment, 20.0f);

    float NewValue = TestFactionManager->GetFactionRelationValue(
        EFactionType::BeiyangGovernment, EFactionType::NationalistGovernment);

    // 關係值應該有所改變（注意：初始可能是負數，修改後應該增加）
    if (NewValue == 0.0f)
    {
        return CreateTestResult(TestName, false, TEXT("Relation value modification failed"));
    }

    return CreateTestResult(TestName, true);
}

FFactionSystemTestResult UMingRTSFactionSystemTest::TestPlayerFactionSetting()
{
    FString TestName = TEXT("PlayerFactionSetting");

    if (!TestFactionManager)
    {
        return CreateTestResult(TestName, false, TEXT("TestFactionManager is null"));
    }

    // 設置玩家勢力為北洋政府
    TestFactionManager->SetPlayerFaction(EFactionType::BeiyangGovernment);

    if (!TestFactionManager->IsPlayerFaction(EFactionType::BeiyangGovernment))
    {
        return CreateTestResult(TestName, false, TEXT("Failed to set player faction"));
    }

    if (TestFactionManager->GetPlayerFaction() != EFactionType::BeiyangGovernment)
    {
        return CreateTestResult(TestName, false, TEXT("GetPlayerFaction returned wrong value"));
    }

    return CreateTestResult(TestName, true);
}

FFactionSystemTestResult UMingRTSFactionSystemTest::TestVictoryPointsSystem()
{
    FString TestName = TEXT("VictoryPointsSystem");

    if (!TestFactionManager)
    {
        return CreateTestResult(TestName, false, TEXT("TestFactionManager is null"));
    }

    // 設置初始勝利點數
    TestFactionManager->SetVictoryPoints(EFactionType::BeiyangGovernment, 100);

    if (TestFactionManager->GetVictoryPoints(EFactionType::BeiyangGovernment) != 100)
    {
        return CreateTestResult(TestName, false, TEXT("Failed to set victory points"));
    }

    // 增加勝利點數
    TestFactionManager->AddVictoryPoints(EFactionType::BeiyangGovernment, 50);

    if (TestFactionManager->GetVictoryPoints(EFactionType::BeiyangGovernment) != 150)
    {
        return CreateTestResult(TestName, false, TEXT("Failed to add victory points"));
    }

    // 測試領先勢力
    EFactionType Leading = TestFactionManager->GetLeadingFaction();
    if (Leading != EFactionType::BeiyangGovernment)
    {
        return CreateTestResult(TestName, false, TEXT("GetLeadingFaction returned wrong value"));
    }

    return CreateTestResult(TestName, true);
}

FFactionSystemTestResult UMingRTSFactionSystemTest::TestTerritoryControl()
{
    FString TestName = TEXT("TerritoryControl");

    if (!TestFactionManager)
    {
        return CreateTestResult(TestName, false, TEXT("TestFactionManager is null"));
    }

    // 添加領土
    TestFactionManager->AddTerritory(EFactionType::BeiyangGovernment, 5);
    if (TestFactionManager->GetTerritoryCount(EFactionType::BeiyangGovernment) < 5)
    {
        return CreateTestResult(TestName, false, TEXT("Failed to add territory"));
    }

    // 移除領土
    TestFactionManager->RemoveTerritory(EFactionType::BeiyangGovernment, 2);
    if (TestFactionManager->GetTerritoryCount(EFactionType::BeiyangGovernment) < 3)
    {
        return CreateTestResult(TestName, false, TEXT("Failed to remove territory correctly"));
    }

    return CreateTestResult(TestName, true);
}

FFactionSystemTestResult UMingRTSFactionSystemTest::TestWarDeclaration()
{
    FString TestName = TEXT("WarDeclaration");

    if (!TestFactionManager)
    {
        return CreateTestResult(TestName, false, TEXT("TestFactionManager is null"));
    }

    // 宣戰
    TestFactionManager->DeclareWar(EFactionType::BeiyangGovernment, EFactionType::FengtianClique);

    EFactionRelationType Relation = TestFactionManager->GetFactionRelation(
        EFactionType::BeiyangGovernment, EFactionType::FengtianClique);

    if (Relation != EFactionRelationType::War)
    {
        return CreateTestResult(TestName, false, TEXT("War declaration failed"));
    }

    return CreateTestResult(TestName, true);
}

FFactionSystemTestResult UMingRTSFactionSystemTest::TestAllianceFormation()
{
    FString TestName = TEXT("AllianceFormation");

    if (!TestFactionManager)
    {
        return CreateTestResult(TestName, false, TEXT("TestFactionManager is null"));
    }

    // 建立同盟
    TestFactionManager->FormAlliance(EFactionType::ZhiliClique, EFactionType::FengtianClique);

    EFactionRelationType Relation = TestFactionManager->GetFactionRelation(
        EFactionType::ZhiliClique, EFactionType::FengtianClique);

    if (Relation != EFactionRelationType::Ally)
    {
        return CreateTestResult(TestName, false, TEXT("Alliance formation failed"));
    }

    // 驗證同盟列表
    TArray<EFactionType> Allies = TestFactionManager->GetAllies(EFactionType::ZhiliClique);
    if (!Allies.Contains(EFactionType::FengtianClique))
    {
        return CreateTestResult(TestName, false, TEXT("FengtianClique not in allies list"));
    }

    return CreateTestResult(TestName, true);
}

FFactionSystemTestResult UMingRTSFactionSystemTest::TestFactionDefeatAndRevival()
{
    FString TestName = TEXT("FactionDefeatAndRevival");

    if (!TestFactionManager)
    {
        return CreateTestResult(TestName, false, TEXT("TestFactionManager is null"))
    }

    // 擊敗勢力
    TestFactionManager->DefeatFaction(EFactionType::AnhuiClique);

    if (TestFactionManager->IsFactionAlive(EFactionType::AnhuiClique))
    {
        return CreateTestResult(TestName, false, TEXT("Defeated faction still marked as alive"));
    }

    if (TestFactionManager->GetFactionState(EFactionType::AnhuiClique) != EFactionState::Defeated)
    {
        return CreateTestResult(TestName, false, TEXT("Faction state not set to defeated"));
    }

    // 復興勢力
    TestFactionManager->ReviveFaction(EFactionType::AnhuiClique);

    if (!TestFactionManager->IsFactionAlive(EFactionType::AnhuiClique))
    {
        return CreateTestResult(TestName, false, TEXT("Revived faction not marked as alive"));
    }

    return CreateTestResult(TestName, true);
}

FFactionSystemTestResult UMingRTSFactionSystemTest::TestUniqueUnits()
{
    FString TestName = TEXT("UniqueUnits");

    if (!TestFactionManager)
    {
        return CreateTestResult(TestName, false, TEXT("TestFactionManager is null"));
    }

    // 驗證每個勢力都有專屬單位
    TArray<EFactionType> AllFactions = TestFactionManager->GetAllRegisteredFactions();
    for (EFactionType Faction : AllFactions)
    {
        FFactionData Data = TestFactionManager->GetFactionData(Faction);
        if (Data.UniqueUnits.Num() < 2)
        {
            return CreateTestResult(TestName, false, 
                FString::Printf(TEXT("Faction %s has less than 2 unique units"), *Data.FactionName));
        }

        // 驗證單位屬性
        for (const auto& Unit : Data.UniqueUnits)
        {
            if (Unit.UnitName.IsEmpty())
            {
                return CreateTestResult(TestName, false, TEXT("Unit name is empty"));
            }
            if (Unit.BaseHealth <= 0)
            {
                return CreateTestResult(TestName, false, TEXT("Unit health is invalid"));
            }
        }
    }

    return CreateTestResult(TestName, true);
}

FFactionSystemTestResult UMingRTSFactionSystemTest::TestSpecialMechanics()
{
    FString TestName = TEXT("SpecialMechanics");

    if (!TestFactionManager)
    {
        return CreateTestResult(TestName, false, TEXT("TestFactionManager is null"));
    }

    // 驗證核心勢力有特色機制
    FFactionData BeiyangData = TestFactionManager->GetFactionData(EFactionType::BeiyangGovernment);
    bool bHasCentralLegitimacy = false;
    for (const auto& Mechanic : BeiyangData.SpecialMechanics)
    {
        if (Mechanic.MechanicType == EFactionSpecialMechanic::CentralLegitimacy)
        {
            bHasCentralLegitimacy = true;
            break;
        }
    }

    if (!bHasCentralLegitimacy)
    {
        return CreateTestResult(TestName, false, TEXT("BeiyangGovernment missing CentralLegitimacy mechanic"));
    }

    // 驗證共產黨有人民戰爭機制
    FFactionData CommunistData = TestFactionManager->GetFactionData(EFactionType::ChineseCommunistParty);
    bool bHasPeoplesWar = false;
    for (const auto& Mechanic : CommunistData.SpecialMechanics)
    {
        if (Mechanic.MechanicType == EFactionSpecialMechanic::PeoplesWar)
        {
            bHasPeoplesWar = true;
            break;
        }
    }

    if (!bHasPeoplesWar)
    {
        return CreateTestResult(TestName, false, TEXT("ChineseCommunistParty missing PeoplesWar mechanic"));
    }

    return CreateTestResult(TestName, true);
}

FFactionSystemTestResult UMingRTSFactionSystemTest::TestAIConfiguration()
{
    FString TestName = TEXT("AIConfiguration");

    if (!TestFactionManager)
    {
        return CreateTestResult(TestName, false, TEXT("TestFactionManager is null"));
    }

    FFactionData Data = TestFactionManager->GetFactionData(EFactionType::BeiyangGovernment);
    if (Data.AIConfig.PrimaryStrategy == EFactionAIStrategy::BalancedApproach &&
        Data.AIConfig.Aggressiveness == 0.5f)
    {
        // 這可能意味著AI配置未被正確設置
        // 北洋應該有較低的好戰性和外交傾向
        UE_LOG(LogTemp, Warning, TEXT("AI configuration may need review for BeiyangGovernment"));
    }

    // 驗證可以修改AI策略
    TestFactionManager->SetFactionAIStrategy(EFactionType::BeiyangGovernment, EFactionAIStrategy::AggressiveExpansion);
    EFactionAIStrategy NewStrategy = TestFactionManager->GetFactionAIStrategy(EFactionType::BeiyangGovernment);

    if (NewStrategy != EFactionAIStrategy::AggressiveExpansion)
    {
        return CreateTestResult(TestName, false, TEXT("Failed to change AI strategy"));
    }

    return CreateTestResult(TestName, true);
}

FFactionSystemTestResult UMingRTSFactionSystemTest::TestTurnSystem()
{
    FString TestName = TEXT("TurnSystem");

    if (!TestFactionManager)
    {
        return CreateTestResult(TestName, false, TEXT("TestFactionManager is null"));
    }

    // 簽署非侵略條約
    TestFactionManager->SignNonAggressionPact(EFactionType::BeiyangGovernment, EFactionType::JinClique, 5);

    // 模擬回合
    for (int32 i = 1; i <= 5; ++i)
    {
        TestFactionManager->OnTurnStart(i);
        TestFactionManager->OnTurnEnd(i);
    }

    return CreateTestResult(TestName, true);
}

FFactionSystemTestResult UMingRTSFactionSystemTest::TestBeiyangGovernment()
{
    FString TestName = TEXT("BeiyangGovernmentSpecific");

    if (!TestFactionManager)
    {
        return CreateTestResult(TestName, false, TEXT("TestFactionManager is null"));
    }

    FFactionData Data = TestFactionManager->GetFactionData(EFactionType::BeiyangGovernment);

    // 驗證難度為容易
    if (Data.Difficulty != EFactionDifficulty::Easy)
    {
        return CreateTestResult(TestName, false, TEXT("BeiyangGovernment should be Easy difficulty"));
    }

    // 驗證起始地區為北京
    if (Data.StartingConfig.PrimaryRegion != EStartingRegion::Beijing)
    {
        return CreateTestResult(TestName, false, TEXT("BeiyangGovernment should start in Beijing"));
    }

    // 驗證起始資源較高
    if (Data.StartingConfig.StartingGold < 1500)
    {
        return CreateTestResult(TestName, false, TEXT("BeiyangGovernment should have high starting gold"));
    }

    // 驗證顏色為深紅色
    if (Data.FactionColor.R < 0.5f)
    {
        return CreateTestResult(TestName, false, TEXT("BeiyangGovernment color should be deep red"));
    }

    return CreateTestResult(TestName, true);
}

FFactionSystemTestResult UMingRTSFactionSystemTest::TestChineseCommunistParty()
{
    FString TestName = TEXT("ChineseCommunistPartySpecific");

    if (!TestFactionManager)
    {
        return CreateTestResult(TestName, false, TEXT("TestFactionManager is null"));
    }

    FFactionData Data = TestFactionManager->GetFactionData(EFactionType::ChineseCommunistParty);

    // 驗證難度為困難
    if (Data.Difficulty != EFactionDifficulty::Hard)
    {
        return CreateTestResult(TestName, false, TEXT("ChineseCommunistParty should be Hard difficulty"));
    }

    // 驗證起始資源較低
    if (Data.StartingConfig.StartingGold > 1000)
    {
        return CreateTestResult(TestName, false, TEXT("ChineseCommunistParty should have low starting resources"));
    }

    // 驗證有人民戰爭機制
    bool bHasMechanic = false;
    for (const auto& Mechanic : Data.SpecialMechanics)
    {
        if (Mechanic.MechanicType == EFactionSpecialMechanic::PeoplesWar)
        {
            bHasMechanic = true;
            break;
        }
    }

    if (!bHasMechanic)
    {
        return CreateTestResult(TestName, false, TEXT("ChineseCommunistParty should have PeoplesWar mechanic"));
    }

    return CreateTestResult(TestName, true);
}

FFactionSystemTestResult UMingRTSFactionSystemTest::TestXinjiangFaction()
{
    FString TestName = TEXT("XinjiangFactionSpecific");

    if (!TestFactionManager)
    {
        return CreateTestResult(TestName, false, TEXT("TestFactionManager is null"));
    }

    FFactionData Data = TestFactionManager->GetFactionData(EFactionType::XinjiangFaction);

    // 驗證難度為極難
    if (Data.Difficulty != EFactionDifficulty::VeryHard)
    {
        return CreateTestResult(TestName, false, TEXT("XinjiangFaction should be VeryHard difficulty"));
    }

    // 驗證起始資源最少
    if (Data.StartingConfig.StartingGold > 800)
    {
        return CreateTestResult(TestName, false, TEXT("XinjiangFaction should have the lowest starting resources"));
    }

    // 驗證起始人口最少
    if (Data.StartingConfig.StartingPopulation > 15000)
    {
        return CreateTestResult(TestName, false, TEXT("XinjiangFaction should have the lowest starting population"));
    }

    return CreateTestResult(TestName, true);
}

void UMingRTSFactionSystemTest::RunSpecificTest(const FString& TestName)
{
    FFactionSystemTestResult Result;

    if (TestName == TEXT("FactionManagerCreation"))
    {
        Result = TestFactionManagerCreation();
    }
    else if (TestName == TEXT("All12FactionsInitialized"))
    {
        Result = TestAll12FactionsInitialized();
    }
    else if (TestName == TEXT("FactionDataIntegrity"))
    {
        Result = TestFactionDataIntegrity();
    }
    else if (TestName == TEXT("FactionRelations"))
    {
        Result = TestFactionRelations();
    }
    else if (TestName == TEXT("PlayerFactionSetting"))
    {
        Result = TestPlayerFactionSetting();
    }
    else if (TestName == TEXT("VictoryPointsSystem"))
    {
        Result = TestVictoryPointsSystem();
    }
    else if (TestName == TEXT("TerritoryControl"))
    {
        Result = TestTerritoryControl();
    }
    else if (TestName == TEXT("WarDeclaration"))
    {
        Result = TestWarDeclaration();
    }
    else if (TestName == TEXT("AllianceFormation"))
    {
        Result = TestAllianceFormation();
    }
    else if (TestName == TEXT("FactionDefeatAndRevival"))
    {
        Result = TestFactionDefeatAndRevival();
    }
    else if (TestName == TEXT("UniqueUnits"))
    {
        Result = TestUniqueUnits();
    }
    else if (TestName == TEXT("SpecialMechanics"))
    {
        Result = TestSpecialMechanics();
    }
    else if (TestName == TEXT("AIConfiguration"))
    {
        Result = TestAIConfiguration();
    }
    else if (TestName == TEXT("TurnSystem"))
    {
        Result = TestTurnSystem();
    }
    else
    {
        Result = CreateTestResult(TestName, false, TEXT("Unknown test name"));
    }

    LogTestResult(Result);
}
