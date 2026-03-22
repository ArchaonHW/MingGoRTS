#include "MingGoRTSTechTreeTest.h"
#include "Engine/World.h"

UMingGoRTSTechTreeTest::UMingGoRTSTechTreeTest()
{
    TechTreeSystem = nullptr;
    PassedTests = 0;
    FailedTests = 0;
}

void UMingGoRTSTechTreeTest::InitializeTest()
{
    ResetTestCounters();
    
    // 創建科技樹系統實例
    TechTreeSystem = NewObject<UMingGoRTSTechTree>();
    if (TechTreeSystem)
    {
        TechTreeSystem->InitializeTechTree();
        AddTestResult(TEXT("科技樹系統初始化"), true, TEXT("科技樹系統成功初始化"));
    }
    else
    {
        AddTestResult(TEXT("科技樹系統初始化"), false, TEXT("無法創建科技樹系統實例"));
    }
}

bool UMingGoRTSTechTreeTest::TestTechTreeInitialization()
{
    if (!TechTreeSystem)
    {
        AddTestResult(TEXT("科技樹初始化測試"), false, TEXT("科技樹系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 檢查所有科技類別
    TArray<ETechCategory> Categories = TechTreeSystem->GetAllTechCategories();
    if (Categories.Num() != 10) // 預期10個類別
    {
        AddTestResult(TEXT("科技類別數量檢查"), false, FString::Printf(TEXT("預期10個類別，實際%d個"), Categories.Num()));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("科技類別數量檢查"), true, TEXT("科技類別數量正確"));
    }

    // 檢查每個類別都有科技
    for (ETechCategory Category : Categories)
    {
        TArray<FTechNode> CategoryTechs = TechTreeSystem->GetTechsByCategory(Category);
        if (CategoryTechs.Num() == 0)
        {
            AddTestResult(TEXT("類別科技檢查"), false, FString::Printf(TEXT("類別 %s 沒有科技"), *StaticEnum<ETechCategory>()->GetValueAsString(Category)));
            bAllPassed = false;
        }
    }

    if (bAllPassed)
    {
        AddTestResult(TEXT("科技樹初始化測試"), true, TEXT("科技樹初始化正常"));
    }

    return bAllPassed;
}

bool UMingGoRTSTechTreeTest::TestTechNodeCreation()
{
    if (!TechTreeSystem)
    {
        AddTestResult(TEXT("科技節點創建測試"), false, TEXT("科技樹系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 測試獲取基礎軍事科技
    FTechNode BasicTraining = TechTreeSystem->GetTechNode(TEXT("TECH_Military_BasicTraining_XXXX"));
    if (!VerifyTechNodeData(BasicTraining, TEXT("基礎軍事訓練"), ETechCategory::Military))
    {
        AddTestResult(TEXT("基礎軍事科技檢查"), false, TEXT("基礎軍事科技數據不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("基礎軍事科技檢查"), true, TEXT("基礎軍事科技數據正確"));
    }

    // 測試獲取基礎經濟科技
    FTechNode BasicTrade = TechTreeSystem->GetTechNode(TEXT("TECH_Economic_BasicTrade_XXXX"));
    if (!VerifyTechNodeData(BasicTrade, TEXT("基礎貿易"), ETechCategory::Economic))
    {
        AddTestResult(TEXT("基礎經濟科技檢查"), false, TEXT("基礎經濟科技數據不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("基礎經濟科技檢查"), true, TEXT("基礎經濟科技數據正確"));
    }

    // 測試不存在的科技
    FTechNode NonExistentTech = TechTreeSystem->GetTechNode(TEXT("NON_EXISTENT_TECH"));
    if (NonExistentTech.TechID.IsEmpty())
    {
        AddTestResult(TEXT("不存在科技檢查"), true, TEXT("正確處理不存在的科技"));
    }
    else
    {
        AddTestResult(TEXT("不存在科技檢查"), false, TEXT("未正確處理不存在的科技"));
        bAllPassed = false;
    }

    if (bAllPassed)
    {
        AddTestResult(TEXT("科技節點創建測試"), true, TEXT("科技節點創建測試通過"));
    }

    return bAllPassed;
}

bool UMingGoRTSTechTreeTest::TestTechResearchFlow()
{
    if (!TechTreeSystem)
    {
        AddTestResult(TEXT("科技研發流程測試"), false, TEXT("科技樹系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 獲取一個可研發的基礎科技
    FString TechID = GetRandomTechID(ETechCategory::Military);
    if (TechID.IsEmpty())
    {
        AddTestResult(TEXT("科技研發流程測試"), false, TEXT("無法獲取測試科技"));
        return false;
    }

    // 測試開始研發
    bool bStarted = TechTreeSystem->StartResearch(TechID, TEXT("TestLab"));
    if (!bStarted)
    {
        AddTestResult(TEXT("開始研發測試"), false, TEXT("無法開始研發"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("開始研發測試"), true, TEXT("成功開始研發"));
    }

    // 檢查研發狀態
    if (!VerifyResearchStatus(TechID, ETechStatus::Researching))
    {
        AddTestResult(TEXT("研發狀態檢查"), false, TEXT("研發狀態不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("研發狀態檢查"), true, TEXT("研發狀態正確"));
    }

    // 獲取研發進度
    FTechResearchProgress Progress = TechTreeSystem->GetResearchProgress(TechID);
    if (Progress.TechID != TechID || Progress.Progress <= 0.0f)
    {
        AddTestResult(TEXT("研發進度檢查"), false, TEXT("研發進度不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("研發進度檢查"), true, TEXT("研發進度正確"));
    }

    // 測試暫停研發
    bool bPaused = TechTreeSystem->PauseResearch(TechID);
    if (!bPaused)
    {
        AddTestResult(TEXT("暫停研發測試"), false, TEXT("無法暫停研發"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("暫停研發測試"), true, TEXT("成功暫停研發"));
    }

    // 檢查暫停後狀態
    if (!VerifyResearchStatus(TechID, ETechStatus::Available))
    {
        AddTestResult(TEXT("暫停後狀態檢查"), false, TEXT("暫停後狀態不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("暫停後狀態檢查"), true, TEXT("暫停後狀態正確"));
    }

    // 重新開始研發
    TechTreeSystem->StartResearch(TechID, TEXT("TestLab"));

    // 測試取消研發
    bool bCancelled = TechTreeSystem->CancelResearch(TechID);
    if (!bCancelled)
    {
        AddTestResult(TEXT("取消研發測試"), false, TEXT("無法取消研發"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("取消研發測試"), true, TEXT("成功取消研發"));
    }

    if (bAllPassed)
    {
        AddTestResult(TEXT("科技研發流程測試"), true, TEXT("科技研發流程測試通過"));
    }

    return bAllPassed;
}

bool UMingGoRTSTechTreeTest::TestPrerequisiteChecking()
{
    if (!TechTreeSystem)
    {
        AddTestResult(TEXT("前置條件檢查測試"), false, TEXT("科技樹系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 測試基礎科技（無前置條件）
    TArray<FTechNode> BasicTechs = TechTreeSystem->GetTechsByCategory(ETechCategory::Military);
    bool bFoundBasicTech = false;
    for (const FTechNode& Tech : BasicTechs)
    {
        if (Tech.Tier == ETechTier::Basic)
        {
            if (TechTreeSystem->CheckPrerequisites(Tech.TechID))
            {
                bFoundBasicTech = true;
                break;
            }
        }
    }

    if (!bFoundBasicTech)
    {
        AddTestResult(TEXT("基礎科技前置條件檢查"), false, TEXT("基礎科技應該無前置條件"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("基礎科技前置條件檢查"), true, TEXT("基礎科技前置條件正確"));
    }

    // 測試進階科技（有前置條件）
    TArray<FTechNode> AdvancedTechs = TechTreeSystem->GetTechsByCategory(ETechCategory::Military);
    bool bFoundAdvancedTech = false;
    for (const FTechNode& Tech : AdvancedTechs)
    {
        if (Tech.Tier == ETechTier::Intermediate)
        {
            if (!TechTreeSystem->CheckPrerequisites(Tech.TechID))
            {
                bFoundAdvancedTech = true;
                break;
            }
        }
    }

    if (!bFoundAdvancedTech)
    {
        AddTestResult(TEXT("進階科技前置條件檢查"), false, TEXT("進階科技應該有前置條件"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("進階科技前置條件檢查"), true, TEXT("進階科技前置條件正確"));
    }

    return bAllPassed;
}

bool UMingGoRTSTechTreeTest::TestTechEffectApplication()
{
    if (!TechTreeSystem)
    {
        AddTestResult(TEXT("科技效果應用測試"), false, TEXT("科技樹系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 獲取一個有效果的科技
    FString TechID = GetRandomTechID(ETechCategory::Military);
    if (TechID.IsEmpty())
    {
        AddTestResult(TEXT("科技效果應用測試"), false, TEXT("無法獲取測試科技"));
        return false;
    }

    FTechNode Tech = TechTreeSystem->GetTechNode(TechID);
    if (Tech.Effects.Num() == 0)
    {
        AddTestResult(TEXT("科技效果檢查"), false, TEXT("測試科技應該有效果"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("科技效果檢查"), true, FString::Printf(TEXT("科技有 %d 個效果"), Tech.Effects.Num()));
    }

    // 直接完成科技以測試效果應用
    bool bCompleted = TechTreeSystem->CompleteResearch(TechID);
    if (!bCompleted)
    {
        AddTestResult(TEXT("科技完成測試"), false, TEXT("無法完成科技"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("科技完成測試"), true, TEXT("成功完成科技"));
    }

    // 檢查是否在已完成列表中
    TArray<FString> CompletedTechs = TechTreeSystem->GetCompletedTechs();
    if (!CompletedTechs.Contains(TechID))
    {
        AddTestResult(TEXT("已完成科技檢查"), false, TEXT("完成科技未在列表中"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("已完成科技檢查"), true, TEXT("完成科技在列表中"));
    }

    return bAllPassed;
}

bool UMingGoRTSTechTreeTest::TestTechUnlocking()
{
    if (!TechTreeSystem)
    {
        AddTestResult(TEXT("科技解鎖機制測試"), false, TEXT("科技樹系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 獲取一個鎖定的進階科技
    TArray<FTechNode> AdvancedTechs = TechTreeSystem->GetTechsByCategory(ETechCategory::Military);
    FString LockedTechID;
    for (const FTechNode& Tech : AdvancedTechs)
    {
        if (Tech.Tier == ETechTier::Intermediate && Tech.Status == ETechStatus::Locked)
        {
            LockedTechID = Tech.TechID;
            break;
        }
    }

    if (LockedTechID.IsEmpty())
    {
        AddTestResult(TEXT("鎖定科技檢查"), false, TEXT("無法找到鎖定的進階科技"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("鎖定科技檢查"), true, TEXT("找到鎖定的進階科技"));
    }

    // 完成前置科技
    TArray<FTechNode> BasicTechs = TechTreeSystem->GetTechsByCategory(ETechCategory::Military);
    for (const FTechNode& Tech : BasicTechs)
    {
        if (Tech.Tier == ETechTier::Basic)
        {
            TechTreeSystem->CompleteResearch(Tech.TechID);
        }
    }

    // 檢查是否解鎖
    bool bUnlocked = TechTreeSystem->UnlockTech(LockedTechID);
    if (!bUnlocked)
    {
        AddTestResult(TEXT("科技解鎖測試"), false, TEXT("無法解鎖科技"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("科技解鎖測試"), true, TEXT("成功解鎖科技"));
    }

    return bAllPassed;
}

bool UMingGoRTSTechTreeTest::TestConcurrentResearchLimit()
{
    if (!TechTreeSystem)
    {
        AddTestResult(TEXT("並行研發限制測試"), false, TEXT("科技樹系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 獲取當前配置
    FTechTreeConfig Config = TechTreeSystem->GetTechTreeConfig();
    int32 MaxConcurrent = Config.MaxConcurrentResearch;

    // 獲取多個基礎科技
    TArray<FString> BasicTechIDs;
    TArray<FTechNode> BasicTechs = TechTreeSystem->GetTechsByCategory(ETechCategory::Military);
    for (const FTechNode& Tech : BasicTechs)
    {
        if (Tech.Tier == ETechTier::Basic && Tech.Status == ETechStatus::Available)
        {
            BasicTechIDs.Add(Tech.TechID);
            if (BasicTechIDs.Num() >= MaxConcurrent + 1)
            {
                break;
            }
        }
    }

    if (BasicTechIDs.Num() < MaxConcurrent + 1)
    {
        AddTestResult(TEXT("並行研發測試準備"), false, TEXT("無法獲取足夠的測試科技"));
        bAllPassed = false;
        return bAllPassed;
    }

    // 開始最大並行數量的研發
    int32 StartedCount = 0;
    for (int32 i = 0; i < MaxConcurrent; ++i)
    {
        if (TechTreeSystem->StartResearch(BasicTechIDs[i], FString::Printf(TEXT("Lab_%d"), i)))
        {
            StartedCount++;
        }
    }

    if (StartedCount != MaxConcurrent)
    {
        AddTestResult(TEXT("最大並行研發測試"), false, FString::Printf(TEXT("預期開始 %d 個，實際 %d 個"), MaxConcurrent, StartedCount));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("最大並行研發測試"), true, TEXT("最大並行研發正常"));
    }

    // 嘗試開始超額的研發
    bool bExtraStarted = TechTreeSystem->StartResearch(BasicTechIDs[MaxConcurrent], TEXT("ExtraLab"));
    if (bExtraStarted)
    {
        AddTestResult(TEXT("超額並行研發測試"), false, TEXT("不應該能開始超額研發"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("超額並行研發測試"), true, TEXT("正確拒絕超額研發"));
    }

    // 清理研發
    for (const FString& TechID : BasicTechIDs)
    {
        TechTreeSystem->CancelResearch(TechID);
    }

    return bAllPassed;
}

bool UMingGoRTSTechTreeTest::TestTechTreeConfiguration()
{
    if (!TechTreeSystem)
    {
        AddTestResult(TEXT("科技樹配置測試"), false, TEXT("科技樹系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 創建測試配置
    FTechTreeConfig TestConfig = CreateTestConfig();

    // 應用配置
    TechTreeSystem->UpdateTechTreeConfig(TestConfig);

    // 驗證配置應用
    if (!VerifyConfigApplication(TestConfig))
    {
        AddTestResult(TEXT("配置應用測試"), false, TEXT("配置應用不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("配置應用測試"), true, TEXT("配置應用正確"));
    }

    return bAllPassed;
}

bool UMingGoRTSTechTreeTest::TestSaveAndLoad()
{
    if (!TechTreeSystem)
    {
        AddTestResult(TEXT("保存和載入測試"), false, TEXT("科技樹系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 完成一個科技
    FString TechID = GetRandomTechID(ETechCategory::Military);
    if (!TechID.IsEmpty())
    {
        TechTreeSystem->CompleteResearch(TechID);
    }

    // 保存數據
    bool bSaved = TechTreeSystem->SaveTechTreeData(TEXT("TestSave"));
    if (!bSaved)
    {
        AddTestResult(TEXT("保存測試"), false, TEXT("無法保存科技樹數據"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("保存測試"), true, TEXT("成功保存科技樹數據"));
    }

    // 重置科技樹
    TechTreeSystem->ResetTechTree();
    TechTreeSystem->InitializeTechTree();

    // 載入數據
    bool bLoaded = TechTreeSystem->LoadTechTreeData(TEXT("TestSave"));
    if (!bLoaded)
    {
        AddTestResult(TEXT("載入測試"), false, TEXT("無法載入科技樹數據"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("載入測試"), true, TEXT("成功載入科技樹數據"));
    }

    // 驗證載入的數據
    if (!TechID.IsEmpty())
    {
        TArray<FString> CompletedTechs = TechTreeSystem->GetCompletedTechs();
        if (!CompletedTechs.Contains(TechID))
        {
            AddTestResult(TEXT("載入數據驗證"), false, TEXT("載入的數據不正確"));
            bAllPassed = false;
        }
        else
        {
            AddTestResult(TEXT("載入數據驗證"), true, TEXT("載入的數據正確"));
        }
    }

    return bAllPassed;
}

bool UMingGoRTSTechTreeTest::TestRepublicanEraTechs()
{
    if (!TechTreeSystem)
    {
        AddTestResult(TEXT("民國時期特色科技測試"), false, TEXT("科技樹系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 檢查軍事科技
    TArray<FTechNode> MilitaryTechs = TechTreeSystem->GetTechsByCategory(ETechCategory::Military);
    if (MilitaryTechs.Num() == 0)
    {
        AddTestResult(TEXT("軍事科技檢查"), false, TEXT("軍事科技不存在"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("軍事科技檢查"), true, FString::Printf(TEXT("找到 %d 個軍事科技"), MilitaryTechs.Num()));
    }

    // 檢查經濟科技
    TArray<FTechNode> EconomicTechs = TechTreeSystem->GetTechsByCategory(ETechCategory::Economic);
    if (EconomicTechs.Num() == 0)
    {
        AddTestResult(TEXT("經濟科技檢查"), false, TEXT("經濟科技不存在"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("經濟科技檢查"), true, FString::Printf(TEXT("找到 %d 個經濟科技"), EconomicTechs.Num()));
    }

    // 檢查歷史背景
    bool bFoundHistoricalContext = false;
    for (const FTechNode& Tech : MilitaryTechs)
    {
        if (!Tech.HistoricalContext.IsEmpty())
        {
            bFoundHistoricalContext = true;
            break;
        }
    }

    if (!bFoundHistoricalContext)
    {
        AddTestResult(TEXT("歷史背景檢查"), false, TEXT("科技缺少歷史背景"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("歷史背景檢查"), true, TEXT("科技包含歷史背景"));
    }

    return bAllPassed;
}

bool UMingGoRTSTechTreeTest::RunAllTests()
{
    InitializeTest();
    
    bool bAllPassed = true;
    
    bAllPassed &= TestTechTreeInitialization();
    bAllPassed &= TestTechNodeCreation();
    bAllPassed &= TestTechResearchFlow();
    bAllPassed &= TestPrerequisiteChecking();
    bAllPassed &= TestTechEffectApplication();
    bAllPassed &= TestTechUnlocking();
    bAllPassed &= TestConcurrentResearchLimit();
    bAllPassed &= TestTechTreeConfiguration();
    bAllPassed &= TestSaveAndLoad();
    bAllPassed &= TestRepublicanEraTechs();
    
    // 添加總體測試結果
    if (bAllPassed)
    {
        AddTestResult(TEXT("所有測試"), true, FString::Printf(TEXT("所有測試通過，通過：%d，失敗：%d"), PassedTests, FailedTests));
    }
    else
    {
        AddTestResult(TEXT("所有測試"), false, FString::Printf(TEXT("部分測試失敗，通過：%d，失敗：%d"), PassedTests, FailedTests));
    }
    
    return bAllPassed;
}

FString UMingGoRTSTechTreeTest::GetTestResults() const
{
    FString Results = TEXT("科技樹系統測試結果\n");
    Results += FString::Printf(TEXT("通過測試：%d\n"), PassedTests);
    Results += FString::Printf(TEXT("失敗測試：%d\n"), FailedTests);
    Results += TEXT("詳細結果：\n");
    
    for (const FString& Result : TestResults)
    {
        Results += Result + TEXT("\n");
    }
    
    return Results;
}

void UMingGoRTSTechTreeTest::CleanupTest()
{
    if (TechTreeSystem)
    {
        TechTreeSystem = nullptr;
    }
    
    TestResults.Empty();
    ResetTestCounters();
}

void UMingGoRTSTechTreeTest::AddTestResult(const FString& TestName, bool bPassed, const FString& Message)
{
    FString Result = FString::Printf(TEXT("[%s] %s"), bPassed ? TEXT("通過") : TEXT("失敗"), *TestName);
    if (!Message.IsEmpty())
    {
        Result += FString::Printf(TEXT(" - %s"), *Message);
    }
    
    TestResults.Add(Result);
    
    if (bPassed)
    {
        PassedTests++;
    }
    else
    {
        FailedTests++;
    }
}

FTechNode UMingGoRTSTechTreeTest::CreateTestTechNode(const FString& TechID, const FString& TechName, ETechCategory Category)
{
    FTechNode TestNode;
    TestNode.TechID = TechID;
    TestNode.TechName = TechName;
    TestNode.TechDescription = FString::Printf(TEXT("測試科技：%s"), *TechName);
    TestNode.Category = Category;
    TestNode.Tier = ETechTier::Basic;
    TestNode.Status = ETechStatus::Available;
    TestNode.ResearchCost.TimeCost = 300.0f;
    TestNode.ResearchCost.RequiredScientists = 1;
    
    return TestNode;
}

bool UMingGoRTSTechTreeTest::VerifyTechNodeData(const FTechNode& TechNode, const FString& ExpectedName, ETechCategory ExpectedCategory)
{
    return TechNode.TechName == ExpectedName && TechNode.Category == ExpectedCategory;
}

bool UMingGoRTSTechTreeTest::VerifyResearchStatus(const FString& TechID, ETechStatus ExpectedStatus)
{
    FTechNode Tech = TechTreeSystem->GetTechNode(TechID);
    return Tech.Status == ExpectedStatus;
}

FString UMingGoRTSTechTreeTest::GetRandomTechID(ETechCategory Category) const
{
    TArray<FTechNode> CategoryTechs = TechTreeSystem->GetTechsByCategory(Category);
    if (CategoryTechs.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, CategoryTechs.Num());
        return CategoryTechs[RandomIndex].TechID;
    }
    return TEXT("");
}

void UMingGoRTSTechTreeTest::ResetTestCounters()
{
    PassedTests = 0;
    FailedTests = 0;
    TestResults.Empty();
}

FTechTreeConfig UMingGoRTSTechTreeTest::CreateTestConfig()
{
    FTechTreeConfig TestConfig;
    TestConfig.GlobalResearchSpeed = 2.0f;
    TestConfig.MaxConcurrentResearch = 2;
    TestConfig.bEnableTechSharing = true;
    TestConfig.TechSharingEfficiency = 0.75f;
    
    // 設置類別加成
    TestConfig.CategoryResearchBonus.Add(ETechCategory::Military, 1.5f);
    TestConfig.CategoryResearchBonus.Add(ETechCategory::Economic, 1.3f);
    
    // 設置等級加成
    TestConfig.TierResearchBonus.Add(ETechTier::Basic, 1.2f);
    TestConfig.TierResearchBonus.Add(ETechTier::Intermediate, 1.1f);
    
    return TestConfig;
}

bool UMingGoRTSTechTreeTest::VerifyConfigApplication(const FTechTreeConfig& ExpectedConfig)
{
    FTechTreeConfig CurrentConfig = TechTreeSystem->GetTechTreeConfig();
    
    return FMath::Abs(CurrentConfig.GlobalResearchSpeed - ExpectedConfig.GlobalResearchSpeed) < 0.01f &&
           CurrentConfig.MaxConcurrentResearch == ExpectedConfig.MaxConcurrentResearch &&
           CurrentConfig.bEnableTechSharing == ExpectedConfig.bEnableTechSharing;
}
