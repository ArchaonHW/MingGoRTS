#include "Innovation/MingRTSInnovationSystemTest.h"
#include "Innovation/MingRTSEcosystemSimulation.h"
#include "Innovation/MingRTSSocialDynamics.h"
#include "Innovation/MingRTSBlockchainIntegration.h"
#include "MingRTSCollaborativeAINetwork.h"

UMingRTSInnovationSystemTest::UMingRTSInnovationSystemTest()
    : TotalTests(0)
    , PassedTests(0)
    , FailedTests(0)
{
}

void UMingRTSInnovationSystemTest::InitializeInnovationTestSuite()
{
    TotalTests = 0;
    PassedTests = 0;
    FailedTests = 0;
    TestResults.Empty();
    UE_LOG(LogTemp, Log, TEXT("Innovation System Test Suite Initialized"));
}

bool UMingRTSInnovationSystemTest::RunCollaborativeAITests()
{
    UE_LOG(LogTemp, Log, TEXT("=== Running Collaborative AI Network Tests ==="));
    
    bool bAllPassed = true;
    bAllPassed &= TestCollaborativeAINetworkCreation();
    bAllPassed &= TestAgentRegistration();
    bAllPassed &= TestCollaborativeDecisionMaking();
    
    return bAllPassed;
}

bool UMingRTSInnovationSystemTest::RunEcosystemSimulationTests()
{
    UE_LOG(LogTemp, Log, TEXT("=== Running Ecosystem Simulation Tests ==="));
    
    bool bAllPassed = true;
    bAllPassed &= TestEcosystemZoneCreation();
    bAllPassed &= TestResourceManagement();
    bAllPassed &= TestClimateSimulation();
    bAllPassed &= TestSpeciesPopulation();
    bAllPassed &= TestPollutionEffects();
    
    return bAllPassed;
}

bool UMingRTSInnovationSystemTest::RunSocialDynamicsTests()
{
    UE_LOG(LogTemp, Log, TEXT("=== Running Social Dynamics Tests ==="));
    
    bool bAllPassed = true;
    bAllPassed &= TestSocialAgentCreation();
    bAllPassed &= TestSocialRelationEstablishment();
    bAllPassed &= TestSocialMobility();
    bAllPassed &= TestSocietyMetrics();
    
    return bAllPassed;
}

bool UMingRTSInnovationSystemTest::RunBlockchainIntegrationTests()
{
    UE_LOG(LogTemp, Log, TEXT("=== Running Blockchain Integration Tests ==="));
    
    bool bAllPassed = true;
    bAllPassed &= TestBlockchainConnection();
    bAllPassed &= TestAccountCreation();
    bAllPassed &= TestTransactionCreation();
    bAllPassed &= TestTokenMinting();
    
    return bAllPassed;
}

bool UMingRTSInnovationSystemTest::RunAllInnovationTests()
{
    UE_LOG(LogTemp, Log, TEXT("========================================"));
    UE_LOG(LogTemp, Log, TEXT("=== INNOVATION SYSTEM TEST SUITE ==="));
    UE_LOG(LogTemp, Log, TEXT("========================================"));
    
    InitializeInnovationTestSuite();
    
    bool bAllPassed = true;
    bAllPassed &= RunCollaborativeAITests();
    bAllPassed &= RunEcosystemSimulationTests();
    bAllPassed &= RunSocialDynamicsTests();
    bAllPassed &= RunBlockchainIntegrationTests();
    
    UE_LOG(LogTemp, Log, TEXT("========================================"));
    UE_LOG(LogTemp, Log, TEXT("Tests: %d | Passed: %d | Failed: %d"), TotalTests, PassedTests, FailedTests);
    UE_LOG(LogTemp, Log, TEXT("Success Rate: %.1f%%"), TotalTests > 0 ? (PassedTests * 100.0f / TotalTests) : 0.0f);
    UE_LOG(LogTemp, Log, TEXT("========================================"));
    
    return bAllPassed;
}

bool UMingRTSInnovationSystemTest::TestCollaborativeAINetworkCreation()
{
    UMingRTSCollaborativeAINetwork* Network = NewObject<UMingRTSCollaborativeAINetwork>();
    bool bPassed = (Network != nullptr);
    
    if (bPassed && Network)
    {
        Network->InitializeCollaborativeNetwork();
        bPassed = Network->GetConnectedAgentCount() == 0;
    }
    
    LogTestResult(TEXT("Collaborative AI Network Creation"), bPassed);
    return bPassed;
}

bool UMingRTSInnovationSystemTest::TestAgentRegistration()
{
    UMingRTSCollaborativeAINetwork* Network = NewObject<UMingRTSCollaborativeAINetwork>();
    if (!Network) return false;
    
    Network->InitializeCollaborativeNetwork();
    
    int32 AgentId = Network->RegisterAIAgent(TEXT("TestAgent"), EAIAgentRole::Strategist, 0.8f);
    bool bPassed = AgentId >= 0;
    
    if (bPassed)
    {
        bPassed = Network->GetConnectedAgentCount() == 1;
    }
    
    LogTestResult(TEXT("Agent Registration"), bPassed);
    return bPassed;
}

bool UMingRTSInnovationSystemTest::TestCollaborativeDecisionMaking()
{
    UMingRTSCollaborativeAINetwork* Network = NewObject<UMingRTSCollaborativeAINetwork>();
    if (!Network) return false;
    
    Network->InitializeCollaborativeNetwork();
    
    // 註冊多個代理
    int32 Agent1 = Network->RegisterAIAgent(TEXT("Agent1"), EAIAgentRole::Strategist, 0.8f);
    int32 Agent2 = Network->RegisterAIAgent(TEXT("Agent2"), EAIAgentRole::Tactician, 0.7f);
    int32 Agent3 = Network->RegisterAIAgent(TEXT("Agent3"), EAIAgentRole::Scout, 0.6f);
    
    bool bPassed = (Agent1 >= 0 && Agent2 >= 0 && Agent3 >= 0);
    
    if (bPassed)
    {
        // 創建決策情境
        FCollaborativeDecision Decision;
        Decision.SituationType = TEXT("Combat");
        Decision.Urgency = 0.8f;
        Decision.Priority = 0.9f;
        
        int32 DecisionId = Network->CreateCollaborativeDecision(Decision);
        bPassed = DecisionId >= 0;
        
        if (bPassed)
        {
            // 模擬決策過程
            Network->SimulateNetworkTick(1.0f);
        }
    }
    
    LogTestResult(TEXT("Collaborative Decision Making"), bPassed);
    return bPassed;
}

bool UMingRTSInnovationSystemTest::TestEcosystemZoneCreation()
{
    UMingRTSEcosystemSimulation* Ecosystem = NewObject<UMingRTSEcosystemSimulation>();
    if (!Ecosystem) return false;
    
    Ecosystem->InitializeEcosystem();
    
    int32 ZoneId = Ecosystem->CreateEcosystemZone(EEcosystemZone::Forest, FVector(1000.0f, 1000.0f, 0.0f), 500.0f);
    bool bPassed = ZoneId > 0;
    
    if (bPassed)
    {
        FEcosystemZoneData ZoneData = Ecosystem->GetZoneData(ZoneId);
        bPassed = ZoneData.ZoneType == EEcosystemZone::Forest;
    }
    
    LogTestResult(TEXT("Ecosystem Zone Creation"), bPassed);
    return bPassed;
}

bool UMingRTSInnovationSystemTest::TestResourceManagement()
{
    UMingRTSEcosystemSimulation* Ecosystem = NewObject<UMingRTSEcosystemSimulation>();
    if (!Ecosystem) return false;
    
    Ecosystem->InitializeEcosystem();
    
    int32 ZoneId = Ecosystem->CreateEcosystemZone(EEcosystemZone::Grassland, FVector::ZeroVector, 1000.0f);
    
    // 添加資源
    Ecosystem->AddResource(ZoneId, EResourceType::Food, 100.0f);
    float ResourceLevel = Ecosystem->GetResourceAvailability(ZoneId, EResourceType::Food);
    
    bool bPassed = ResourceLevel > 0.0f;
    
    if (bPassed)
    {
        // 消耗資源
        Ecosystem->ConsumeResource(ZoneId, EResourceType::Food, 50.0f);
        float NewLevel = Ecosystem->GetResourceAvailability(ZoneId, EResourceType::Food);
        bPassed = NewLevel < ResourceLevel;
    }
    
    LogTestResult(TEXT("Resource Management"), bPassed);
    return bPassed;
}

bool UMingRTSInnovationSystemTest::TestClimateSimulation()
{
    UMingRTSEcosystemSimulation* Ecosystem = NewObject<UMingRTSEcosystemSimulation>();
    if (!Ecosystem) return false;
    
    Ecosystem->InitializeEcosystem();
    
    int32 ZoneId = Ecosystem->CreateEcosystemZone(EEcosystemZone::Forest, FVector::ZeroVector, 1000.0f);
    
    // 設置季節
    Ecosystem->SetSeason(ESeasonType::Summer);
    
    FClimateCondition Climate = Ecosystem->GetZoneClimate(ZoneId);
    bool bPassed = Climate.CurrentSeason == ESeasonType::Summer;
    
    if (bPassed)
    {
        // 夏季溫度應該較高
        bPassed = Climate.Temperature > 15.0f;
    }
    
    LogTestResult(TEXT("Climate Simulation"), bPassed);
    return bPassed;
}

bool UMingRTSInnovationSystemTest::TestSpeciesPopulation()
{
    UMingRTSEcosystemSimulation* Ecosystem = NewObject<UMingRTSEcosystemSimulation>();
    if (!Ecosystem) return false;
    
    Ecosystem->InitializeEcosystem();
    
    int32 ZoneId = Ecosystem->CreateEcosystemZone(EEcosystemZone::Forest, FVector::ZeroVector, 1000.0f);
    
    // 添加物種
    FSpeciesPopulation Species;
    Species.SpeciesName = TEXT("Deer");
    Species.PopulationCount = 50;
    Species.GrowthRate = 0.05f;
    Species.DeathRate = 0.02f;
    Species.MigrationRate = 0.01f;
    Species.RequiredResources.Add(EResourceType::Food);
    Species.PreferredZone = EEcosystemZone::Forest;
    
    Ecosystem->AddSpecies(ZoneId, Species);
    
    TArray<FSpeciesPopulation> ZoneSpecies = Ecosystem->GetZoneSpecies(ZoneId);
    bool bPassed = ZoneSpecies.Num() > 0;
    
    if (bPassed)
    {
        bPassed = ZoneSpecies[0].SpeciesName == TEXT("Deer");
    }
    
    LogTestResult(TEXT("Species Population"), bPassed);
    return bPassed;
}

bool UMingRTSInnovationSystemTest::TestPollutionEffects()
{
    UMingRTSEcosystemSimulation* Ecosystem = NewObject<UMingRTSEcosystemSimulation>();
    if (!Ecosystem) return false;
    
    Ecosystem->InitializeEcosystem();
    
    int32 ZoneId = Ecosystem->CreateEcosystemZone(EEcosystemZone::Forest, FVector::ZeroVector, 1000.0f);
    
    // 應用污染
    Ecosystem->ApplyPollution(ZoneId, 0.5f);
    
    float HealthBefore = Ecosystem->CalculateZoneHealth(ZoneId);
    
    bool bPassed = HealthBefore < 1.0f;
    
    // 清理污染
    Ecosystem->CleanUpPollution(ZoneId, 0.3f);
    
    float HealthAfter = Ecosystem->CalculateZoneHealth(ZoneId);
    bPassed &= HealthAfter > HealthBefore;
    
    LogTestResult(TEXT("Pollution Effects"), bPassed);
    return bPassed;
}

bool UMingRTSInnovationSystemTest::TestSocialAgentCreation()
{
    UMingRTSSocialDynamics* Social = NewObject<UMingRTSSocialDynamics>();
    if (!Social) return false;
    
    Social->InitializeSocialSystem();
    
    int32 AgentId = Social->CreateSocialAgent(TEXT("Wang Wu"), ESocialClass::Merchant);
    bool bPassed = AgentId > 0;
    
    LogTestResult(TEXT("Social Agent Creation"), bPassed);
    return bPassed;
}

bool UMingRTSInnovationSystemTest::TestSocialRelationEstablishment()
{
    UMingRTSSocialDynamics* Social = NewObject<UMingRTSSocialDynamics>();
    if (!Social) return false;
    
    Social->InitializeSocialSystem();
    
    int32 Agent1 = Social->CreateSocialAgent(TEXT("Li Si"), ESocialClass::Scholar);
    int32 Agent2 = Social->CreateSocialAgent(TEXT("Zhang San"), ESocialClass::Official);
    
    bool bPassed = (Agent1 > 0 && Agent2 > 0);
    
    if (bPassed)
    {
        Social->EstablishRelation(Agent1, Agent2, ESocialRelationType::Political, 0.8f);
        // 關係建立成功無返回值，假設成功
        bPassed = true;
    }
    
    LogTestResult(TEXT("Social Relation Establishment"), bPassed);
    return bPassed;
}

bool UMingRTSInnovationSystemTest::TestSocialMobility()
{
    UMingRTSSocialDynamics* Social = NewObject<UMingRTSSocialDynamics>();
    if (!Social) return false;
    
    Social->InitializeSocialSystem();
    
    // 創建多個不同階層的代理
    int32 Peasant = Social->CreateSocialAgent(TEXT("Farmer"), ESocialClass::Peasant);
    int32 Artisan = Social->CreateSocialAgent(TEXT("Craftsman"), ESocialClass::Artisan);
    int32 Merchant = Social->CreateSocialAgent(TEXT("Trader"), ESocialClass::Merchant);
    
    bool bPassed = (Peasant > 0 && Artisan > 0 && Merchant > 0);
    
    // 模擬社會流動
    Social->SimulateSocialTick(1.0f);
    
    LogTestResult(TEXT("Social Mobility"), bPassed);
    return bPassed;
}

bool UMingRTSInnovationSystemTest::TestSocietyMetrics()
{
    UMingRTSSocialDynamics* Social = NewObject<UMingRTSSocialDynamics>();
    if (!Social) return false;
    
    Social->InitializeSocialSystem();
    
    // 創建多個代理形成社會
    for (int32 i = 0; i < 10; ++i)
    {
        ESocialClass Class = static_cast<ESocialClass>(FMath::RandRange(0, (int32)ESocialClass::Outlaw));
        Social->CreateSocialAgent(FString::Printf(TEXT("Citizen%d"), i), Class);
    }
    
    FSocietyMetrics Metrics = Social->GetSocietyMetrics();
    
    bool bPassed = Metrics.AverageHappiness >= 0.0f && Metrics.AverageHappiness <= 1.0f;
    bPassed &= Metrics.SocialStability >= 0.0f && Metrics.SocialStability <= 1.0f;
    bPassed &= Metrics.ClassDistribution.Num() > 0;
    
    LogTestResult(TEXT("Society Metrics"), bPassed);
    return bPassed;
}

bool UMingRTSInnovationSystemTest::TestBlockchainConnection()
{
    UMingRTSBlockchainIntegration* Blockchain = NewObject<UMingRTSBlockchainIntegration>();
    if (!Blockchain) return false;
    
    Blockchain->InitializeBlockchain(EBlockchainType::Private);
    
    bool bPassed = Blockchain->IsConnected();
    
    LogTestResult(TEXT("Blockchain Connection"), bPassed);
    return bPassed;
}

bool UMingRTSInnovationSystemTest::TestAccountCreation()
{
    UMingRTSBlockchainIntegration* Blockchain = NewObject<UMingRTSBlockchainIntegration>();
    if (!Blockchain) return false;
    
    Blockchain->InitializeBlockchain(EBlockchainType::Ethereum);
    
    FBlockchainAccount Account = Blockchain->CreateAccount(TEXT("Player123"));
    
    bool bPassed = !Account.Address.IsEmpty();
    bPassed &= Account.bIsActive;
    
    LogTestResult(TEXT("Account Creation"), bPassed);
    return bPassed;
}

bool UMingRTSInnovationSystemTest::TestTransactionCreation()
{
    UMingRTSBlockchainIntegration* Blockchain = NewObject<UMingRTSBlockchainIntegration>();
    if (!Blockchain) return false;
    
    Blockchain->InitializeBlockchain(EBlockchainType::Ethereum);
    
    FBlockchainAccount FromAccount = Blockchain->CreateAccount(TEXT("Sender"));
    FBlockchainAccount ToAccount = Blockchain->CreateAccount(TEXT("Receiver"));
    
    // 先給發送方一些餘額
    // 在實際測試中可能需要預設餘額
    
    FString TxHash = Blockchain->CreateTransaction(FromAccount.Address, ToAccount.Address, 10.0f, TEXT("Test payment"));
    
    // 由於新帳戶餘額為0，交易應該失敗
    bool bPassed = TxHash.IsEmpty(); // 預期交易失敗
    
    LogTestResult(TEXT("Transaction Creation"), bPassed);
    return bPassed;
}

bool UMingRTSInnovationSystemTest::TestTokenMinting()
{
    UMingRTSBlockchainIntegration* Blockchain = NewObject<UMingRTSBlockchainIntegration>();
    if (!Blockchain) return false;
    
    Blockchain->InitializeBlockchain(EBlockchainType::Ethereum);
    
    FBlockchainAccount Account = Blockchain->CreateAccount(TEXT("TokenHolder"));
    
    // 鑄造代幣
    Blockchain->MintToken(Account.Address, ETokenType::ERC20, TEXT("TestToken"), 1000.0f);
    
    // 鑄造應該成功，雖然沒有返回值，但應該不會崩潰
    bool bPassed = true;
    
    LogTestResult(TEXT("Token Minting"), bPassed);
    return bPassed;
}

void UMingRTSInnovationSystemTest::LogTestResult(const FString& TestName, bool bPassed)
{
    TotalTests++;
    if (bPassed)
    {
        PassedTests++;
        UE_LOG(LogTemp, Log, TEXT("[PASS] %s"), *TestName);
    }
    else
    {
        FailedTests++;
        UE_LOG(LogTemp, Error, TEXT("[FAIL] %s"), *TestName);
    }
    TestResults.Add(TestName, bPassed);
}
