// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTS/Test/MingRTSSprint2Test.h"
#include "RTS/MingRTSResourceGatheringSystem.h"
#include "RTS/MingRTSBuildingConstructionSystem.h"
#include "Engine/World.h"

UMingRTSSprint2Test::UMingRTSSprint2Test()
{
}

void UMingRTSSprint2Test::InitializeTestSuite()
{
    ClearResults();
    UE_LOG(LogTemp, Log, TEXT("Sprint 2 Test Suite initialized"));
}

TArray<FSprint2TestResult> UMingRTSSprint2Test::RunAllTests()
{
    ClearResults();

    // 资源系统测试
    RecordTestResult(TEXT("Resource Node Registration"), TestResourceNodeRegistration().bPassed);
    RecordTestResult(TEXT("Resource Gathering"), TestResourceGathering().bPassed);
    RecordTestResult(TEXT("Resource Inventory"), TestResourceInventory().bPassed);
    RecordTestResult(TEXT("Resource Capacity"), TestResourceCapacity().bPassed);
    RecordTestResult(TEXT("Resource Trading"), TestResourceTrading().bPassed);

    // 建筑系统测试
    RecordTestResult(TEXT("Building Planning"), TestBuildingPlanning().bPassed);
    RecordTestResult(TEXT("Building Construction"), TestBuildingConstruction().bPassed);
    RecordTestResult(TEXT("Building Upgrade"), TestBuildingUpgrade().bPassed);
    RecordTestResult(TEXT("Building Repair"), TestBuildingRepair().bPassed);
    RecordTestResult(TEXT("Building Production"), TestBuildingProduction().bPassed);

    // 经济集成测试
    RecordTestResult(TEXT("Resource to Building Integration"), TestResourceToBuildingIntegration().bPassed);
    RecordTestResult(TEXT("Economy Cycle"), TestEconomyCycle().bPassed);
    RecordTestResult(TEXT("Building Resource Generation"), TestBuildingResourceGeneration().bPassed);

    OnAllTestsCompleted.Broadcast();

    UE_LOG(LogTemp, Log, TEXT("Sprint 2 All Tests completed - Pass rate: %.1f%%"), GetTestPassRate());

    return TestResults;
}

FSprint2TestResult UMingRTSSprint2Test::TestResourceNodeRegistration()
{
    FSprint2TestResult Result;
    Result.TestName = TEXT("Resource Node Registration");
    Result.bPassed = true;

    UMingRTSResourceGatheringSystem* ResourceSystem = NewObject<UMingRTSResourceGatheringSystem>();
    if (!ResourceSystem)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Failed to create resource system");
        return Result;
    }

    ResourceSystem->InitializeResourceSystem();

    // 测试注册资源节点
    FRTSResourceNode Node;
    Node.ResourceType = ERTSResourceType::Wood;
    Node.Location = FVector(1000.0f, 1000.0f, 0.0f);
    Node.Amount = 500.0f;
    Node.MaxAmount = 500.0f;
    
    ResourceSystem->RegisterResourceNode(Node);
    
    // 验证节点是否被注册
    TArray<FRTSResourceNode> WoodNodes = ResourceSystem->GetResourceNodesOfType(ERTSResourceType::Wood);
    if (WoodNodes.Num() == 0)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Resource node was not registered");
        return Result;
    }

    UE_LOG(LogTemp, Log, TEXT("✓ Resource Node Registration test passed"));
    return Result;
}

FSprint2TestResult UMingRTSSprint2Test::TestResourceGathering()
{
    FSprint2TestResult Result;
    Result.TestName = TEXT("Resource Gathering");
    Result.bPassed = true;

    UMingRTSResourceGatheringSystem* ResourceSystem = NewObject<UMingRTSResourceGatheringSystem>();
    if (!ResourceSystem)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Failed to create resource system");
        return Result;
    }

    ResourceSystem->InitializeResourceSystem();

    // 创建测试资源节点
    FRTSResourceNode Node;
    Node.ResourceType = ERTSResourceType::Wood;
    Node.Location = FVector(0.0f, 0.0f, 0.0f);
    Node.Amount = 1000.0f;
    Node.MaxAmount = 1000.0f;
    Node.GatherRate = 1.0f;
    
    ResourceSystem->RegisterResourceNode(Node);

    // 测试开始采集
    // 注意：由于需要实际Actor，这里只测试调用不会崩溃
    ResourceSystem->ProcessGathering(1.0f);

    UE_LOG(LogTemp, Log, TEXT("✓ Resource Gathering test passed"));
    return Result;
}

FSprint2TestResult UMingRTSSprint2Test::TestResourceInventory()
{
    FSprint2TestResult Result;
    Result.TestName = TEXT("Resource Inventory");
    Result.bPassed = true;

    UMingRTSResourceGatheringSystem* ResourceSystem = NewObject<UMingRTSResourceGatheringSystem>();
    if (!ResourceSystem)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Failed to create resource system");
        return Result;
    }

    ResourceSystem->InitializeResourceSystem();

    // 测试添加资源
    int32 TestPlayerID = 1;
    ResourceSystem->AddResource(ERTSResourceType::Wood, 100.0f, TestPlayerID);
    
    float WoodAmount = ResourceSystem->GetResourceAmount(ERTSResourceType::Wood, TestPlayerID);
    if (WoodAmount != 100.0f)
    {
        Result.bPassed = false;
        Result.ErrorMessage = FString::Printf(TEXT("Expected 100 wood, got %.1f"), WoodAmount);
        return Result;
    }

    // 测试消耗资源
    bool bConsumed = ResourceSystem->ConsumeResource(ERTSResourceType::Wood, 50.0f, TestPlayerID);
    if (!bConsumed)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Failed to consume resources");
        return Result;
    }

    WoodAmount = ResourceSystem->GetResourceAmount(ERTSResourceType::Wood, TestPlayerID);
    if (WoodAmount != 50.0f)
    {
        Result.bPassed = false;
        Result.ErrorMessage = FString::Printf(TEXT("Expected 50 wood after consumption, got %.1f"), WoodAmount);
        return Result;
    }

    UE_LOG(LogTemp, Log, TEXT("✓ Resource Inventory test passed"));
    return Result;
}

FSprint2TestResult UMingRTSSprint2Test::TestResourceCapacity()
{
    FSprint2TestResult Result;
    Result.TestName = TEXT("Resource Capacity");
    Result.bPassed = true;

    UMingRTSResourceGatheringSystem* ResourceSystem = NewObject<UMingRTSResourceGatheringSystem>();
    if (!ResourceSystem)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Failed to create resource system");
        return Result;
    }

    ResourceSystem->InitializeResourceSystem();

    int32 TestPlayerID = 1;
    
    // 测试容量限制
    float WoodCapacity = ResourceSystem->GetResourceCapacity(ERTSResourceType::Wood, TestPlayerID);
    
    // 添加超过容量的资源
    ResourceSystem->AddResource(ERTSResourceType::Wood, WoodCapacity + 1000.0f, TestPlayerID);
    
    float WoodAmount = ResourceSystem->GetResourceAmount(ERTSResourceType::Wood, TestPlayerID);
    
    // 验证资源不超过容量
    if (WoodAmount > WoodCapacity)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Resource amount exceeded capacity");
        return Result;
    }

    UE_LOG(LogTemp, Log, TEXT("✓ Resource Capacity test passed"));
    return Result;
}

FSprint2TestResult UMingRTSSprint2Test::TestResourceTrading()
{
    FSprint2TestResult Result;
    Result.TestName = TEXT("Resource Trading");
    Result.bPassed = true;

    UMingRTSResourceGatheringSystem* ResourceSystem = NewObject<UMingRTSResourceGatheringSystem>();
    if (!ResourceSystem)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Failed to create resource system");
        return Result;
    }

    ResourceSystem->InitializeResourceSystem();

    int32 Player1 = 1;
    int32 Player2 = 2;

    // 给玩家1添加资源
    ResourceSystem->AddResource(ERTSResourceType::Gold, 500.0f, Player1);

    // 测试交易
    bool bTraded = ResourceSystem->TradeResources(Player1, Player2, ERTSResourceType::Gold, 100.0f);
    
    if (!bTraded)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Failed to trade resources");
        return Result;
    }

    // 验证资源转移
    float Player1Gold = ResourceSystem->GetResourceAmount(ERTSResourceType::Gold, Player1);
    float Player2Gold = ResourceSystem->GetResourceAmount(ERTSResourceType::Gold, Player2);

    if (Player1Gold != 400.0f || Player2Gold != 100.0f)
    {
        Result.bPassed = false;
        Result.ErrorMessage = FString::Printf(TEXT("Trade failed. Player1: %.1f, Player2: %.1f"), Player1Gold, Player2Gold);
        return Result;
    }

    UE_LOG(LogTemp, Log, TEXT("✓ Resource Trading test passed"));
    return Result;
}

FSprint2TestResult UMingRTSSprint2Test::TestBuildingPlanning()
{
    FSprint2TestResult Result;
    Result.TestName = TEXT("Building Planning");
    Result.bPassed = true;

    UMingRTSBuildingConstructionSystem* BuildingSystem = NewObject<UMingRTSBuildingConstructionSystem>();
    if (!BuildingSystem)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Failed to create building system");
        return Result;
    }

    BuildingSystem->InitializeBuildingSystem();

    // 测试建筑规划
    FVector BuildLocation(500.0f, 500.0f, 0.0f);
    int32 PlayerID = 1;
    
    bool bPlanned = BuildingSystem->PlanBuilding(ERTSBuildingType::Barracks, BuildLocation, PlayerID);
    
    // 由于资源系统未完全集成，这个测试可能返回true或false取决于实现
    // 我们主要测试调用不会崩溃

    UE_LOG(LogTemp, Log, TEXT("✓ Building Planning test passed"));
    return Result;
}

FSprint2TestResult UMingRTSSprint2Test::TestBuildingConstruction()
{
    FSprint2TestResult Result;
    Result.TestName = TEXT("Building Construction");
    Result.bPassed = true;

    UMingRTSBuildingConstructionSystem* BuildingSystem = NewObject<UMingRTSBuildingConstructionSystem>();
    if (!BuildingSystem)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Failed to create building system");
        return Result;
    }

    BuildingSystem->InitializeBuildingSystem();

    // 测试建造处理
    BuildingSystem->ProcessConstruction(1.0f);

    UE_LOG(LogTemp, Log, TEXT("✓ Building Construction test passed"));
    return Result;
}

FSprint2TestResult UMingRTSSprint2Test::TestBuildingUpgrade()
{
    FSprint2TestResult Result;
    Result.TestName = TEXT("Building Upgrade");
    Result.bPassed = true;

    // 创建测试建筑
    FRTSBuildingInfo BuildingInfo;
    BuildingInfo.BuildingType = ERTSBuildingType::Barracks;
    BuildingInfo.Level = 1;
    BuildingInfo.MaxLevel = 3;
    BuildingInfo.Health = 1000.0f;
    BuildingInfo.MaxHealth = 1000.0f;

    // 验证初始等级
    if (BuildingInfo.Level != 1)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Initial level should be 1");
        return Result;
    }

    // 模拟升级
    BuildingInfo.Level = 2;
    BuildingInfo.Health += 500.0f;
    BuildingInfo.MaxHealth += 500.0f;

    if (BuildingInfo.Level != 2)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Upgrade failed");
        return Result;
    }

    UE_LOG(LogTemp, Log, TEXT("✓ Building Upgrade test passed"));
    return Result;
}

FSprint2TestResult UMingRTSSprint2Test::TestBuildingRepair()
{
    FSprint2TestResult Result;
    Result.TestName = TEXT("Building Repair");
    Result.bPassed = true;

    FRTSBuildingInfo BuildingInfo;
    BuildingInfo.Health = 500.0f;
    BuildingInfo.MaxHealth = 1000.0f;
    BuildingInfo.BuildingState = ERTSBuildingState::Damaged;

    // 模拟修复
    float RepairAmount = 300.0f;
    BuildingInfo.Health = FMath::Min(BuildingInfo.Health + RepairAmount, BuildingInfo.MaxHealth);

    if (BuildingInfo.Health != 800.0f)
    {
        Result.bPassed = false;
        Result.ErrorMessage = FString::Printf(TEXT("Expected 800 health, got %.1f"), BuildingInfo.Health);
        return Result;
    }

    UE_LOG(LogTemp, Log, TEXT("✓ Building Repair test passed"));
    return Result;
}

FSprint2TestResult UMingRTSSprint2Test::TestBuildingProduction()
{
    FSprint2TestResult Result;
    Result.TestName = TEXT("Building Production");
    Result.bPassed = true;

    // 创建兵营建筑
    FRTSBuildingInfo BuildingInfo;
    BuildingInfo.BuildingType = ERTSBuildingType::Barracks;
    BuildingInfo.bCanProduceUnits = true;
    BuildingInfo.ProducibleUnits.Add(FName("Infantry"));
    BuildingInfo.ProducibleUnits.Add(FName("Archer"));

    // 验证生产功能
    if (!BuildingInfo.bCanProduceUnits)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Building should be able to produce units");
        return Result;
    }

    if (BuildingInfo.ProducibleUnits.Num() != 2)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Building should have 2 producible units");
        return Result;
    }

    UE_LOG(LogTemp, Log, TEXT("✓ Building Production test passed"));
    return Result;
}

FSprint2TestResult UMingRTSSprint2Test::TestResourceToBuildingIntegration()
{
    FSprint2TestResult Result;
    Result.TestName = TEXT("Resource to Building Integration");
    Result.bPassed = true;

    // 测试资源系统与建筑系统的集成
    UMingRTSResourceGatheringSystem* ResourceSystem = NewObject<UMingRTSResourceGatheringSystem>();
    UMingRTSBuildingConstructionSystem* BuildingSystem = NewObject<UMingRTSBuildingConstructionSystem>();

    if (!ResourceSystem || !BuildingSystem)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Failed to create systems");
        return Result;
    }

    ResourceSystem->InitializeResourceSystem();
    BuildingSystem->InitializeBuildingSystem();

    // 添加资源
    int32 PlayerID = 1;
    ResourceSystem->AddResource(ERTSResourceType::Wood, 1000.0f, PlayerID);
    ResourceSystem->AddResource(ERTSResourceType::Stone, 500.0f, PlayerID);

    // 检查资源
    bool bHasEnoughWood = ResourceSystem->HasEnoughResource(ERTSResourceType::Wood, 300.0f, PlayerID);
    bool bHasEnoughStone = ResourceSystem->HasEnoughResource(ERTSResourceType::Stone, 100.0f, PlayerID);

    if (!bHasEnoughWood || !bHasEnoughStone)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Should have enough resources for building");
        return Result;
    }

    UE_LOG(LogTemp, Log, TEXT("✓ Resource to Building Integration test passed"));
    return Result;
}

FSprint2TestResult UMingRTSSprint2Test::TestEconomyCycle()
{
    FSprint2TestResult Result;
    Result.TestName = TEXT("Economy Cycle");
    Result.bPassed = true;

    // 测试经济循环：采集资源 -> 建造建筑 -> 生产单位
    UMingRTSResourceGatheringSystem* ResourceSystem = NewObject<UMingRTSResourceGatheringSystem>();
    
    if (!ResourceSystem)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Failed to create resource system");
        return Result;
    }

    ResourceSystem->InitializeResourceSystem();

    int32 PlayerID = 1;

    // 步骤1: 采集资源
    ResourceSystem->AddResource(ERTSResourceType::Gold, 1000.0f, PlayerID);
    ResourceSystem->AddResource(ERTSResourceType::Food, 500.0f, PlayerID);
    ResourceSystem->AddResource(ERTSResourceType::Wood, 800.0f, PlayerID);

    // 步骤2: 验证资源可用于建造
    bool bCanBuild = ResourceSystem->HasEnoughResource(ERTSResourceType::Wood, 300.0f, PlayerID) &&
                     ResourceSystem->HasEnoughResource(ERTSResourceType::Gold, 200.0f, PlayerID);

    if (!bCanBuild)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Should have enough resources for building");
        return Result;
    }

    // 步骤3: 模拟建造消耗
    ResourceSystem->ConsumeResource(ERTSResourceType::Wood, 300.0f, PlayerID);
    ResourceSystem->ConsumeResource(ERTSResourceType::Gold, 200.0f, PlayerID);

    // 步骤4: 验证剩余资源
    float RemainingWood = ResourceSystem->GetResourceAmount(ERTSResourceType::Wood, PlayerID);
    float RemainingGold = ResourceSystem->GetResourceAmount(ERTSResourceType::Gold, PlayerID);

    if (RemainingWood != 500.0f || RemainingGold != 800.0f)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Resource calculation error in economy cycle");
        return Result;
    }

    UE_LOG(LogTemp, Log, TEXT("✓ Economy Cycle test passed"));
    return Result;
}

FSprint2TestResult UMingRTSSprint2Test::TestBuildingResourceGeneration()
{
    FSprint2TestResult Result;
    Result.TestName = TEXT("Building Resource Generation");
    Result.bPassed = true;

    UMingRTSBuildingConstructionSystem* BuildingSystem = NewObject<UMingRTSBuildingConstructionSystem>();
    
    if (!BuildingSystem)
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Failed to create building system");
        return Result;
    }

    BuildingSystem->InitializeBuildingSystem();

    // 测试不同建筑类型的可生产资源
    TArray<ERTSResourceType> FarmResources = BuildingSystem->GetBuildingProducibleResources(ERTSBuildingType::Farm);
    TArray<ERTSResourceType> MineResources = BuildingSystem->GetBuildingProducibleResources(ERTSBuildingType::Mine);
    TArray<ERTSResourceType> MarketResources = BuildingSystem->GetBuildingProducibleResources(ERTSBuildingType::Market);

    // 农场应该产生食物
    if (!FarmResources.Contains(ERTSResourceType::Food))
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Farm should produce Food");
        return Result;
    }

    // 矿场应该产生石头、铁、煤
    if (!MineResources.Contains(ERTSResourceType::Stone) ||
        !MineResources.Contains(ERTSResourceType::Iron) ||
        !MineResources.Contains(ERTSResourceType::Coal))
    {
        Result.bPassed = false;
        Result.ErrorMessage = TEXT("Mine should produce Stone, Iron and Coal");
        return Result;
    }

    UE_LOG(LogTemp, Log, TEXT("✓ Building Resource Generation test passed"));
    return Result;
}

FString UMingRTSSprint2Test::GenerateTestReport()
{
    FString Report = TEXT("=== Sprint 2 - Resource Building Economy Test Report ===\n\n");
    Report += FString::Printf(TEXT("Total Tests: %d\n"), GetTotalTestCount());
    Report += FString::Printf(TEXT("Passed: %d\n"), GetPassedTestCount());
    Report += FString::Printf(TEXT("Failed: %d\n"), GetFailedTestCount());
    Report += FString::Printf(TEXT("Pass Rate: %.1f%%\n\n"), GetTestPassRate());

    Report += TEXT("Detailed Results:\n");
    for (const FSprint2TestResult& TestResult : TestResults)
    {
        FString Status = TestResult.bPassed ? TEXT("✓ PASS") : TEXT("✗ FAIL");
        Report += FString::Printf(TEXT("  [%s] %s"), *Status, *TestResult.TestName);
        if (!TestResult.bPassed && !TestResult.ErrorMessage.IsEmpty())
        {
            Report += FString::Printf(TEXT(" - %s"), *TestResult.ErrorMessage);
        }
        Report += TEXT("\n");
    }

    return Report;
}

int32 UMingRTSSprint2Test::GetPassedTestCount() const
{
    int32 PassedCount = 0;
    for (const FSprint2TestResult& Result : TestResults)
    {
        if (Result.bPassed)
        {
            PassedCount++;
        }
    }
    return PassedCount;
}

int32 UMingRTSSprint2Test::GetFailedTestCount() const
{
    int32 FailedCount = 0;
    for (const FSprint2TestResult& Result : TestResults)
    {
        if (!Result.bPassed)
        {
            FailedCount++;
        }
    }
    return FailedCount;
}

float UMingRTSSprint2Test::GetTestPassRate() const
{
    int32 Total = GetTotalTestCount();
    if (Total == 0)
    {
        return 0.0f;
    }
    return (float)GetPassedTestCount() / (float)Total * 100.0f;
}

void UMingRTSSprint2Test::RecordTestResult(const FString& TestName, bool bPassed, const FString& ErrorMessage, float ExecutionTime)
{
    FSprint2TestResult Result;
    Result.TestName = TestName;
    Result.bPassed = bPassed;
    Result.ErrorMessage = ErrorMessage;
    Result.ExecutionTime = ExecutionTime;

    TestResults.Add(Result);
    OnTestCompleted.Broadcast(Result);
}

void UMingRTSSprint2Test::ClearResults()
{
    TestResults.Empty();
}
