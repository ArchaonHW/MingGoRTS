#include "MingGoRTSProductionTest.h"
#include "Engine/World.h"

UMingGoRTSProductionTest::UMingGoRTSProductionTest()
{
    ProductionSystem = nullptr;
    PassedTests = 0;
    FailedTests = 0;
}

void UMingGoRTSProductionTest::InitializeTest()
{
    ResetTestCounters();
    
    // 創建生產系統實例
    ProductionSystem = NewObject<UMingGoRTSProductionSystem>();
    if (ProductionSystem)
    {
        ProductionSystem->InitializeProductionSystem();
        AddTestResult(TEXT("生產系統初始化"), true, TEXT("生產系統成功初始化"));
    }
    else
    {
        AddTestResult(TEXT("生產系統初始化"), false, TEXT("無法創建生產系統實例"));
    }
}

bool UMingGoRTSProductionTest::TestProductionSystemInitialization()
{
    if (!ProductionSystem)
    {
        AddTestResult(TEXT("生產系統初始化測試"), false, TEXT("生產系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 檢查所有生產類別
    TArray<EProductionCategory> Categories = ProductionSystem->GetAllProductionCategories();
    if (Categories.Num() != 10) // 預期10個類別
    {
        AddTestResult(TEXT("生產類別數量檢查"), false, FString::Printf(TEXT("預期10個類別，實際%d個"), Categories.Num()));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("生產類別數量檢查"), true, TEXT("生產類別數量正確"));
    }

    // 檢查每個類別都有配方
    for (EProductionCategory Category : Categories)
    {
        TArray<FProductionRecipe> CategoryRecipes = ProductionSystem->GetRecipesByCategory(Category);
        if (CategoryRecipes.Num() == 0)
        {
            AddTestResult(TEXT("類別配方檢查"), false, FString::Printf(TEXT("類別 %s 沒有配方"), *StaticEnum<EProductionCategory>()->GetValueAsString(Category)));
            bAllPassed = false;
        }
    }

    if (bAllPassed)
    {
        AddTestResult(TEXT("生產系統初始化測試"), true, TEXT("生產系統初始化正常"));
    }

    return bAllPassed;
}

bool UMingGoRTSProductionTest::TestProductionLineCreation()
{
    if (!ProductionSystem)
    {
        AddTestResult(TEXT("生產線創建測試"), false, TEXT("生產系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 創建測試生產線
    FString ProductionLineID = ProductionSystem->CreateProductionLine(TEXT("測試生產線"), TEXT("TestFactory"));
    if (ProductionLineID.IsEmpty())
    {
        AddTestResult(TEXT("生產線創建"), false, TEXT("無法創建生產線"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("生產線創建"), true, FString::Printf(TEXT("成功創建生產線：%s"), *ProductionLineID));
    }

    // 驗證生產線數據
    FProductionLine ProductionLine = ProductionSystem->GetProductionLine(ProductionLineID);
    if (!VerifyProductionLineData(ProductionLine, TEXT("測試生產線")))
    {
        AddTestResult(TEXT("生產線數據驗證"), false, TEXT("生產線數據不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("生產線數據驗證"), true, TEXT("生產線數據正確"));
    }

    // 檢查對應的生產隊列
    FProductionQueue Queue = ProductionSystem->GetProductionQueue(ProductionLineID);
    if (Queue.QueueID.IsEmpty())
    {
        AddTestResult(TEXT("生產隊列創建"), false, TEXT("生產隊列未正確創建"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("生產隊列創建"), true, TEXT("生產隊列正確創建"));
    }

    return bAllPassed;
}

bool UMingGoRTSProductionTest::TestProductionRecipeCreation()
{
    if (!ProductionSystem)
    {
        AddTestResult(TEXT("生產配方創建測試"), false, TEXT("生產系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 測試獲取軍事配方
    TArray<FProductionRecipe> MilitaryRecipes = ProductionSystem->GetRecipesByCategory(EProductionCategory::Military);
    if (MilitaryRecipes.Num() == 0)
    {
        AddTestResult(TEXT("軍事配方檢查"), false, TEXT("軍事配方不存在"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("軍事配方檢查"), true, FString::Printf(TEXT("找到 %d 個軍事配方"), MilitaryRecipes.Num()));
    }

    // 測試獲取工業配方
    TArray<FProductionRecipe> IndustrialRecipes = ProductionSystem->GetRecipesByCategory(EProductionCategory::Industrial);
    if (IndustrialRecipes.Num() == 0)
    {
        AddTestResult(TEXT("工業配方檢查"), false, TEXT("工業配方不存在"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("工業配方檢查"), true, FString::Printf(TEXT("找到 %d 個工業配方"), IndustrialRecipes.Num()));
    }

    // 測試獲取特定配方
    FString RifleRecipeID = GetRandomRecipeID(EProductionCategory::Military);
    if (!RifleRecipeID.IsEmpty())
    {
        FProductionRecipe RifleRecipe = ProductionSystem->GetProductionRecipe(RifleRecipeID);
        if (!VerifyRecipeData(RifleRecipe, TEXT("步槍生產"), EProductionCategory::Military))
        {
            AddTestResult(TEXT("步槍配方驗證"), false, TEXT("步槍配方數據不正確"));
            bAllPassed = false;
        }
        else
        {
            AddTestResult(TEXT("步槍配方驗證"), true, TEXT("步槍配方數據正確"));
        }
    }

    return bAllPassed;
}

bool UMingGoRTSProductionTest::TestProductionFlow()
{
    if (!ProductionSystem)
    {
        AddTestResult(TEXT("生產流程測試"), false, TEXT("生產系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 創建測試生產線
    FString ProductionLineID = ProductionSystem->CreateProductionLine(TEXT("流程測試線"), TEXT("TestFactory"));
    if (ProductionLineID.IsEmpty())
    {
        AddTestResult(TEXT("生產流程測試"), false, TEXT("無法創建測試生產線"));
        return false;
    }

    // 獲取測試配方
    FString RecipeID = GetRandomRecipeID(EProductionCategory::Military);
    if (RecipeID.IsEmpty())
    {
        AddTestResult(TEXT("生產流程測試"), false, TEXT("無法獲取測試配方"));
        return false;
    }

    // 測試開始生產
    bool bStarted = ProductionSystem->StartProduction(ProductionLineID, RecipeID);
    if (!bStarted)
    {
        AddTestResult(TEXT("開始生產測試"), false, TEXT("無法開始生產"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("開始生產測試"), true, TEXT("成功開始生產"));
    }

    // 檢查生產狀態
    if (!VerifyProductionStatus(ProductionLineID, EProductionStatus::Producing))
    {
        AddTestResult(TEXT("生產狀態檢查"), false, TEXT("生產狀態不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("生產狀態檢查"), true, TEXT("生產狀態正確"));
    }

    // 模擬生產進度
    SimulateTimeProgress(5.0f); // 模擬5秒

    // 檢查生產進度
    FProductionLine ProductionLine = ProductionSystem->GetProductionLine(ProductionLineID);
    if (ProductionLine.Progress <= 0.0f)
    {
        AddTestResult(TEXT("生產進度檢查"), false, TEXT("生產進度未更新"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("生產進度檢查"), true, FString::Printf(TEXT("生產進度：%.1f%%"), ProductionLine.Progress));
    }

    // 測試暫停生產
    bool bPaused = ProductionSystem->PauseProduction(ProductionLineID);
    if (!bPaused)
    {
        AddTestResult(TEXT("暫停生產測試"), false, TEXT("無法暫停生產"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("暫停生產測試"), true, TEXT("成功暫停生產"));
    }

    // 檢查暫停後狀態
    if (!VerifyProductionStatus(ProductionLineID, EProductionStatus::Paused))
    {
        AddTestResult(TEXT("暫停後狀態檢查"), false, TEXT("暫停後狀態不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("暫停後狀態檢查"), true, TEXT("暫停後狀態正確"));
    }

    // 測試恢復生產
    bool bResumed = ProductionSystem->ResumeProduction(ProductionLineID);
    if (!bResumed)
    {
        AddTestResult(TEXT("恢復生產測試"), false, TEXT("無法恢復生產"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("恢復生產測試"), true, TEXT("成功恢復生產"));
    }

    // 測試取消生產
    bool bCancelled = ProductionSystem->CancelProduction(ProductionLineID);
    if (!bCancelled)
    {
        AddTestResult(TEXT("取消生產測試"), false, TEXT("無法取消生產"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("取消生產測試"), true, TEXT("成功取消生產"));
    }

    return bAllPassed;
}

bool UMingGoRTSProductionTest::TestProductionQueueManagement()
{
    if (!ProductionSystem)
    {
        AddTestResult(TEXT("生產隊列管理測試"), false, TEXT("生產系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 創建測試生產線
    FString ProductionLineID = ProductionSystem->CreateProductionLine(TEXT("隊列測試線"), TEXT("TestFactory"));
    if (ProductionLineID.IsEmpty())
    {
        AddTestResult(TEXT("生產隊列管理測試"), false, TEXT("無法創建測試生產線"));
        return false;
    }

    // 獲取多個測試配方
    TArray<FString> RecipeIDs;
    TArray<FProductionRecipe> MilitaryRecipes = ProductionSystem->GetRecipesByCategory(EProductionCategory::Military);
    for (int32 i = 0; i < FMath::Min(3, MilitaryRecipes.Num()); ++i)
    {
        RecipeIDs.Add(MilitaryRecipes[i].RecipeID);
    }

    // 測試添加到隊列
    for (const FString& RecipeID : RecipeIDs)
    {
        bool bAdded = ProductionSystem->AddToProductionQueue(ProductionLineID, RecipeID);
        if (!bAdded)
        {
            AddTestResult(TEXT("添加到隊列測試"), false, FString::Printf(TEXT("無法添加配方 %s 到隊列"), *RecipeID));
            bAllPassed = false;
        }
    }

    if (bAllPassed)
    {
        AddTestResult(TEXT("添加到隊列測試"), true, FString::Printf(TEXT("成功添加 %d 個配方到隊列"), RecipeIDs.Num()));
    }

    // 檢查隊列狀態
    FProductionQueue Queue = ProductionSystem->GetProductionQueue(ProductionLineID);
    if (Queue.RecipeQueue.Num() != RecipeIDs.Num())
    {
        AddTestResult(TEXT("隊列狀態檢查"), false, TEXT("隊列狀態不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("隊列狀態檢查"), true, FString::Printf(TEXT("隊列包含 %d 個配方"), Queue.RecipeQueue.Num()));
    }

    // 測試移除隊列項目
    bool bRemoved = ProductionSystem->RemoveFromProductionQueue(ProductionLineID, 0);
    if (!bRemoved)
    {
        AddTestResult(TEXT("移除隊列項目測試"), false, TEXT("無法移除隊列項目"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("移除隊列項目測試"), true, TEXT("成功移除隊列項目"));
    }

    // 測試清空隊列
    bool bCleared = ProductionSystem->ClearProductionQueue(ProductionLineID);
    if (!bCleared)
    {
        AddTestResult(TEXT("清空隊列測試"), false, TEXT("無法清空隊列"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("清空隊列測試"), true, TEXT("成功清空隊列"));
    }

    return bAllPassed;
}

bool UMingGoRTSProductionTest::TestProductionEfficiencyCalculation()
{
    if (!ProductionSystem)
    {
        AddTestResult(TEXT("生產效率計算測試"), false, TEXT("生產系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 創建測試生產線
    FString ProductionLineID = ProductionSystem->CreateProductionLine(TEXT("效率測試線"), TEXT("TestFactory"));
    if (ProductionLineID.IsEmpty())
    {
        AddTestResult(TEXT("生產效率計算測試"), false, TEXT("無法創建測試生產線"));
        return false;
    }

    // 獲取基礎效率
    float BaseEfficiency = ProductionSystem->CalculateProductionEfficiency(ProductionLineID);
    if (BaseEfficiency <= 0.0f)
    {
        AddTestResult(TEXT("基礎效率檢查"), false, TEXT("基礎效率無效"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("基礎效率檢查"), true, FString::Printf(TEXT("基礎效率：%.2f"), BaseEfficiency));
    }

    // 測試自動化效率加成
    bool bAutomated = ProductionSystem->SetAutomation(ProductionLineID, true);
    if (!bAutomated)
    {
        AddTestResult(TEXT("自動化設置測試"), false, TEXT("無法設置自動化"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("自動化設置測試"), true, TEXT("成功設置自動化"));
    }

    // 檢查自動化後效率
    float AutomatedEfficiency = ProductionSystem->CalculateProductionEfficiency(ProductionLineID);
    if (AutomatedEfficiency <= BaseEfficiency)
    {
        AddTestResult(TEXT("自動化效率檢查"), false, TEXT("自動化未提升效率"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("自動化效率檢查"), true, FString::Printf(TEXT("自動化效率：%.2f"), AutomatedEfficiency));
    }

    // 測試生產時間計算
    FString RecipeID = GetRandomRecipeID(EProductionCategory::Military);
    if (!RecipeID.IsEmpty())
    {
        AddTestResult(TEXT("生產時間計算測試"), false, TEXT("無法獲取測試配方"));
        bAllPassed = false;
    }
    else
    {
        float ProductionTime = ProductionSystem->CalculateProductionTime(ProductionLineID, RecipeID);
        if (ProductionTime <= 0.0f)
        {
            AddTestResult(TEXT("生產時間計算測試"), false, TEXT("生產時間無效"));
            bAllPassed = false;
        }
        else
        {
            AddTestResult(TEXT("生產時間計算測試"), true, FString::Printf(TEXT("生產時間：%.2f秒"), ProductionTime));
        }
    }

    return bAllPassed;
}

bool UMingGoRTSProductionTest::TestProductionPauseResume()
{
    if (!ProductionSystem)
    {
        AddTestResult(TEXT("生產暫停恢復測試"), false, TEXT("生產系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 創建測試生產線
    FString ProductionLineID = ProductionSystem->CreateProductionLine(TEXT("暫停恢復測試線"), TEXT("TestFactory"));
    if (ProductionLineID.IsEmpty())
    {
        AddTestResult(TEXT("生產暫停恢復測試"), false, TEXT("無法創建測試生產線"));
        return false;
    }

    // 開始生產
    FString RecipeID = GetRandomRecipeID(EProductionCategory::Military);
    if (RecipeID.IsEmpty())
    {
        AddTestResult(TEXT("生產暫停恢復測試"), false, TEXT("無法獲取測試配方"));
        return false;
    }

    bool bStarted = ProductionSystem->StartProduction(ProductionLineID, RecipeID);
    if (!bStarted)
    {
        AddTestResult(TEXT("開始生產測試"), false, TEXT("無法開始生產"));
        bAllPassed = false;
    }

    // 測試多重暫停
    bool bPaused1 = ProductionSystem->PauseProduction(ProductionLineID, TEXT("測試暫停1"));
    bool bPaused2 = ProductionSystem->PauseProduction(ProductionLineID, TEXT("測試暫停2"));
    if (bPaused1 && !bPaused2)
    {
        AddTestResult(TEXT("重複暫停測試"), true, TEXT("正確處理重複暫停"));
    }
    else
    {
        AddTestResult(TEXT("重複暫停測試"), false, TEXT("重複暫停處理錯誤"));
        bAllPassed = false;
    }

    // 測試恢復
    bool bResumed = ProductionSystem->ResumeProduction(ProductionLineID);
    if (!bResumed)
    {
        AddTestResult(TEXT("恢復生產測試"), false, TEXT("無法恢復生產"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("恢復生產測試"), true, TEXT("成功恢復生產"));
    }

    // 檢查恢復後狀態
    if (!VerifyProductionStatus(ProductionLineID, EProductionStatus::Producing))
    {
        AddTestResult(TEXT("恢復後狀態檢查"), false, TEXT("恢復後狀態不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("恢復後狀態檢查"), true, TEXT("恢復後狀態正確"));
    }

    return bAllPassed;
}

bool UMingGoRTSProductionTest::TestProductionPriority()
{
    if (!ProductionSystem)
    {
        AddTestResult(TEXT("生產優先級測試"), false, TEXT("生產系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 創建多個測試生產線
    TArray<FString> ProductionLineIDs;
    for (int32 i = 0; i < 3; ++i)
    {
        FString LineID = ProductionSystem->CreateProductionLine(FString::Printf(TEXT("優先級測試線%d"), i), TEXT("TestFactory"));
        if (!LineID.IsEmpty())
        {
            ProductionLineIDs.Add(LineID);
        }
    }

    if (ProductionLineIDs.Num() < 3)
    {
        AddTestResult(TEXT("生產優先級測試"), false, TEXT("無法創建足夠的測試生產線"));
        return false;
    }

    // 設置不同優先級
    TArray<EProductionPriority> Priorities = {
        EProductionPriority::Low,
        EProductionPriority::Normal,
        EProductionPriority::High
    };

    for (int32 i = 0; i < ProductionLineIDs.Num(); ++i)
    {
        bool bSet = ProductionSystem->SetProductionPriority(ProductionLineIDs[i], Priorities[i]);
        if (!bSet)
        {
            AddTestResult(TEXT("設置優先級測試"), false, FString::Printf(TEXT("無法設置生產線 %d 的優先級"), i));
            bAllPassed = false;
        }
        else
        {
            AddTestResult(TEXT("設置優先級測試"), true, FString::Printf(TEXT("成功設置生產線 %d 的優先級"), i));
        }
    }

    // 驗證優先級設置
    for (int32 i = 0; i < ProductionLineIDs.Num(); ++i)
    {
        FProductionLine ProductionLine = ProductionSystem->GetProductionLine(ProductionLineIDs[i]);
        if (ProductionLine.Priority != Priorities[i])
        {
            AddTestResult(TEXT("優先級驗證"), false, FString::Printf(TEXT("生產線 %d 優先級不正確"), i));
            bAllPassed = false;
        }
    }

    if (bAllPassed)
    {
        AddTestResult(TEXT("優先級驗證"), true, TEXT("所有優先級設置正確"));
    }

    return bAllPassed;
}

bool UMingGoRTSProductionTest::TestProductionLineUpgrade()
{
    if (!ProductionSystem)
    {
        AddTestResult(TEXT("生產線升級測試"), false, TEXT("生產系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 創建測試生產線
    FString ProductionLineID = ProductionSystem->CreateProductionLine(TEXT("升級測試線"), TEXT("TestFactory"));
    if (ProductionLineID.IsEmpty())
    {
        AddTestResult(TEXT("生產線升級測試"), false, TEXT("無法創建測試生產線"));
        return false;
    }

    // 測試空閒狀態升級
    bool bUpgraded = ProductionSystem->UpgradeProductionLine(ProductionLineID);
    if (!bUpgraded)
    {
        AddTestResult(TEXT("空閒升級測試"), false, TEXT("無法升級空閒生產線"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("空閒升級測試"), true, TEXT("成功升級空閒生產線"));
    }

    // 開始生產
    FString RecipeID = GetRandomRecipeID(EProductionCategory::Military);
    if (!RecipeID.IsEmpty())
    {
        ProductionSystem->StartProduction(ProductionLineID, RecipeID);
    }

    // 測試生產中升級（應該失敗）
    bool bUpgradeFailed = ProductionSystem->UpgradeProductionLine(ProductionLineID);
    if (bUpgradeFailed)
    {
        AddTestResult(TEXT("生產中升級測試"), true, TEXT("正確拒絕生產中升級"));
    }
    else
    {
        AddTestResult(TEXT("生產中升級測試"), false, TEXT("錯誤允許生產中升級"));
        bAllPassed = false;
    }

    // 取消生產後再測試升級
    ProductionSystem->CancelProduction(ProductionLineID);
    bool bUpgradedAfterCancel = ProductionSystem->UpgradeProductionLine(ProductionLineID);
    if (!bUpgradedAfterCancel)
    {
        AddTestResult(TEXT("取消後升級測試"), false, TEXT("取消後無法升級"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("取消後升級測試"), true, TEXT("取消後成功升級"));
    }

    return bAllPassed;
}

bool UMingGoRTSProductionTest::TestAutomationFeatures()
{
    if (!ProductionSystem)
    {
        AddTestResult(TEXT("自動化功能測試"), false, TEXT("生產系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 創建測試生產線
    FString ProductionLineID = ProductionSystem->CreateProductionLine(TEXT("自動化測試線"), TEXT("TestFactory"));
    if (ProductionLineID.IsEmpty())
    {
        AddTestResult(TEXT("自動化功能測試"), false, TEXT("無法創建測試生產線"));
        return false;
    }

    // 測試啟用自動化
    bool bAutomated = ProductionSystem->SetAutomation(ProductionLineID, true);
    if (!bAutomated)
    {
        AddTestResult(TEXT("啟用自動化測試"), false, TEXT("無法啟用自動化"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("啟用自動化測試"), true, TEXT("成功啟用自動化"));
    }

    // 檢查自動化狀態
    FProductionLine ProductionLine = ProductionSystem->GetProductionLine(ProductionLineID);
    if (!ProductionLine.bIsAutomated)
    {
        AddTestResult(TEXT("自動化狀態檢查"), false, TEXT("自動化狀態不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("自動化狀態檢查"), true, TEXT("自動化狀態正確"));
    }

    // 測試關閉自動化
    bool bDeautomated = ProductionSystem->SetAutomation(ProductionLineID, false);
    if (!bDeautomated)
    {
        AddTestResult(TEXT("關閉自動化測試"), false, TEXT("無法關閉自動化"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("關閉自動化測試"), true, TEXT("成功關閉自動化"));
    }

    // 檢查關閉後狀態
    ProductionLine = ProductionSystem->GetProductionLine(ProductionLineID);
    if (ProductionLine.bIsAutomated)
    {
        AddTestResult(TEXT("關閉後狀態檢查"), false, TEXT("關閉後自動化狀態不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("關閉後狀態檢查"), true, TEXT("關閉後自動化狀態正確"));
    }

    return bAllPassed;
}

bool UMingGoRTSProductionTest::TestRepublicanEraRecipes()
{
    if (!ProductionSystem)
    {
        AddTestResult(TEXT("民國時期特色配方測試"), false, TEXT("生產系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 檢查軍事配方
    TArray<FProductionRecipe> MilitaryRecipes = ProductionSystem->GetRecipesByCategory(EProductionCategory::Military);
    if (MilitaryRecipes.Num() == 0)
    {
        AddTestResult(TEXT("軍事配方檢查"), false, TEXT("軍事配方不存在"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("軍事配方檢查"), true, FString::Printf(TEXT("找到 %d 個軍事配方"), MilitaryRecipes.Num()));
    }

    // 檢查工業配方
    TArray<FProductionRecipe> IndustrialRecipes = ProductionSystem->GetRecipesByCategory(EProductionCategory::Industrial);
    if (IndustrialRecipes.Num() == 0)
    {
        AddTestResult(TEXT("工業配方檢查"), false, TEXT("工業配方不存在"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("工業配方檢查"), true, FString::Printf(TEXT("找到 %d 個工業配方"), IndustrialRecipes.Num()));
    }

    // 檢查歷史背景
    bool bFoundHistoricalContext = false;
    for (const FProductionRecipe& Recipe : MilitaryRecipes)
    {
        if (!Recipe.HistoricalContext.IsEmpty())
        {
            bFoundHistoricalContext = true;
            break;
        }
    }

    if (!bFoundHistoricalContext)
    {
        AddTestResult(TEXT("歷史背景檢查"), false, TEXT("配方缺少歷史背景"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("歷史背景檢查"), true, TEXT("配方包含歷史背景"));
    }

    // 檢查民國特色資源
    bool bFoundRepublicanResources = false;
    for (const FProductionRecipe& Recipe : MilitaryRecipes)
    {
        for (const auto& ResourcePair : Recipe.InputResources)
        {
            if (ResourcePair.Key == EResourceType::Weapons || 
                ResourcePair.Key == EResourceType::Ammunition ||
                ResourcePair.Key == EResourceType::Uniforms)
            {
                bFoundRepublicanResources = true;
                break;
            }
        }
        if (bFoundRepublicanResources) break;
    }

    if (!bFoundRepublicanResources)
    {
        AddTestResult(TEXT("民國資源檢查"), false, TEXT("配方缺少民國特色資源"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("民國資源檢查"), true, TEXT("配方包含民國特色資源"));
    }

    return bAllPassed;
}

bool UMingGoRTSProductionTest::TestProductionStatistics()
{
    if (!ProductionSystem)
    {
        AddTestResult(TEXT("生產統計測試"), false, TEXT("生產系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 獲取生產統計
    FProductionStatistics Stats = ProductionSystem->GetProductionStatistics();
    
    // 檢查統計結構
    if (Stats.TotalProduced.Num() == 0)
    {
        AddTestResult(TEXT("統計結構檢查"), false, TEXT("統計結構無效"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("統計結構檢查"), true, TEXT("統計結構有效"));
    }

    // 檢查初始值
    if (Stats.TotalProductionRuns != 0 || Stats.SuccessfulRuns != 0 || Stats.FailedRuns != 0)
    {
        AddTestResult(TEXT("初始統計值檢查"), false, TEXT("初始統計值應為0"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("初始統計值檢查"), true, TEXT("初始統計值正確"));
    }

    return bAllPassed;
}

bool UMingGoRTSProductionTest::TestProductionConfiguration()
{
    if (!ProductionSystem)
    {
        AddTestResult(TEXT("生產配置測試"), false, TEXT("生產系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 獲取當前配置
    FProductionSystemConfig CurrentConfig = ProductionSystem->GetProductionConfig();
    
    // 創建測試配置
    FProductionSystemConfig TestConfig = CreateTestConfig();
    
    // 應用配置
    ProductionSystem->UpdateProductionConfig(TestConfig);
    
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

bool UMingGoRTSProductionTest::TestSaveAndLoad()
{
    if (!ProductionSystem)
    {
        AddTestResult(TEXT("保存和載入測試"), false, TEXT("生產系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 創建測試數據
    FString ProductionLineID = ProductionSystem->CreateProductionLine(TEXT("保存測試線"), TEXT("TestFactory"));
    if (ProductionLineID.IsEmpty())
    {
        AddTestResult(TEXT("保存和載入測試"), false, TEXT("無法創建測試數據"));
        return false;
    }

    // 保存數據
    bool bSaved = ProductionSystem->SaveProductionData(TEXT("TestSave"));
    if (!bSaved)
    {
        AddTestResult(TEXT("保存測試"), false, TEXT("無法保存生產數據"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("保存測試"), true, TEXT("成功保存生產數據"));
    }

    // 重置系統
    // TODO: 重置生產系統

    // 載入數據
    bool bLoaded = ProductionSystem->LoadProductionData(TEXT("TestSave"));
    if (!bLoaded)
    {
        AddTestResult(TEXT("載入測試"), false, TEXT("無法載入生產數據"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("載入測試"), true, TEXT("成功載入生產數據"));
    }

    return bAllPassed;
}

bool UMingGoRTSProductionTest::RunAllTests()
{
    InitializeTest();
    
    bool bAllPassed = true;
    
    bAllPassed &= TestProductionSystemInitialization();
    bAllPassed &= TestProductionLineCreation();
    bAllPassed &= TestProductionRecipeCreation();
    bAllPassed &= TestProductionFlow();
    bAllPassed &= TestProductionQueueManagement();
    bAllPassed &= TestProductionEfficiencyCalculation();
    bAllPassed &= TestProductionPauseResume();
    bAllPassed &= TestProductionPriority();
    bAllPassed &= TestProductionLineUpgrade();
    bAllPassed &= TestAutomationFeatures();
    bAllPassed &= TestRepublicanEraRecipes();
    bAllPassed &= TestProductionStatistics();
    bAllPassed &= TestProductionConfiguration();
    bAllPassed &= TestSaveAndLoad();
    
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

FString UMingGoRTSProductionTest::GetTestResults() const
{
    FString Results = TEXT("生產製造系統測試結果\n");
    Results += FString::Printf(TEXT("通過測試：%d\n"), PassedTests);
    Results += FString::Printf(TEXT("失敗測試：%d\n"), FailedTests);
    Results += TEXT("詳細結果：\n");
    
    for (const FString& Result : TestResults)
    {
        Results += Result + TEXT("\n");
    }
    
    return Results;
}

void UMingGoRTSProductionTest::CleanupTest()
{
    if (ProductionSystem)
    {
        ProductionSystem = nullptr;
    }
    
    TestResults.Empty();
    ResetTestCounters();
}

void UMingGoRTSProductionTest::AddTestResult(const FString& TestName, bool bPassed, const FString& Message)
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

FProductionLine UMingGoRTSProductionTest::CreateTestProductionLine(const FString& LineName, const FString& BuildingID)
{
    FProductionLine TestLine;
    TestLine.ProductionLineID = FString::Printf(TEXT("TEST_%s"), *LineName);
    TestLine.ProductionLineName = LineName;
    TestLine.BuildingID = BuildingID;
    TestLine.Status = EProductionStatus::Idle;
    TestLine.CurrentEfficiency = 1.0f;
    
    return TestLine;
}

FProductionRecipe UMingGoRTSProductionTest::CreateTestRecipe(const FString& RecipeName, EProductionCategory Category)
{
    FProductionRecipe TestRecipe;
    TestRecipe.RecipeID = FString::Printf(TEXT("TEST_%s"), *RecipeName);
    TestRecipe.RecipeName = RecipeName;
    TestRecipe.Description = FString::Printf(TEXT("測試配方：%s"), *RecipeName);
    TestRecipe.Category = Category;
    TestRecipe.ProductionTime = 300.0f;
    TestRecipe.BaseEfficiency = 1.0f;
    TestRecipe.MinSkillLevel = 0;
    TestRecipe.DefaultQuality = EProductionQuality::Normal;
    TestRecipe.bIsRepeatable = true;
    
    return TestRecipe;
}

bool UMingGoRTSProductionTest::VerifyProductionLineData(const FProductionLine& ProductionLine, const FString& ExpectedName)
{
    return ProductionLine.ProductionLineName == ExpectedName && !ProductionLine.ProductionLineID.IsEmpty();
}

bool UMingGoRTSProductionTest::VerifyRecipeData(const FProductionRecipe& Recipe, const FString& ExpectedName, EProductionCategory ExpectedCategory)
{
    return Recipe.RecipeName == ExpectedName && Recipe.Category == ExpectedCategory;
}

bool UMingGoRTSProductionTest::VerifyProductionStatus(const FString& ProductionLineID, EProductionStatus ExpectedStatus)
{
    FProductionLine ProductionLine = ProductionSystem->GetProductionLine(ProductionLineID);
    return ProductionLine.Status == ExpectedStatus;
}

FString UMingGoRTSProductionTest::GetRandomRecipeID(EProductionCategory Category) const
{
    TArray<FProductionRecipe> CategoryRecipes = ProductionSystem->GetRecipesByCategory(Category);
    if (CategoryRecipes.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, CategoryRecipes.Num());
        return CategoryRecipes[RandomIndex].RecipeID;
    }
    return TEXT("");
}

void UMingGoRTSProductionTest::ResetTestCounters()
{
    PassedTests = 0;
    FailedTests = 0;
    TestResults.Empty();
}

FProductionSystemConfig UMingGoRTSProductionTest::CreateTestConfig()
{
    FProductionSystemConfig TestConfig;
    TestConfig.GlobalProductionSpeed = 2.0f;
    TestConfig.MaxOverclockEfficiency = 2.0f;
    TestConfig.MaintenancePenalty = 0.5f;
    TestConfig.bEnableAutoDistribution = true;
    TestConfig.bEnableQualityControl = true;
    TestConfig.MaxQueueSize = 15;
    
    // 設置類別加成
    TestConfig.CategoryBonus.Add(EProductionCategory::Military, 1.5f);
    TestConfig.CategoryBonus.Add(EProductionCategory::Industrial, 1.3f);
    
    return TestConfig;
}

bool UMingGoRTSProductionTest::VerifyConfigApplication(const FProductionSystemConfig& ExpectedConfig)
{
    FProductionSystemConfig CurrentConfig = ProductionSystem->GetProductionConfig();
    
    return FMath::Abs(CurrentConfig.GlobalProductionSpeed - ExpectedConfig.GlobalProductionSpeed) < 0.01f &&
           CurrentConfig.MaxOverclockEfficiency == ExpectedConfig.MaxOverclockEfficiency &&
           CurrentConfig.MaintenancePenalty == ExpectedConfig.MaintenancePenalty &&
           CurrentConfig.bEnableAutoDistribution == ExpectedConfig.bEnableAutoDistribution &&
           CurrentConfig.bEnableQualityControl == ExpectedConfig.bEnableQualityControl &&
           CurrentConfig.MaxQueueSize == ExpectedConfig.MaxQueueSize;
}

bool UMingGoRTSProductionTest::WaitForProductionCompletion(const FString& ProductionLineID, float TimeoutSeconds)
{
    float ElapsedTime = 0.0f;
    float DeltaTime = 0.1f;
    
    while (ElapsedTime < TimeoutSeconds)
    {
        ProductionSystem->UpdateProductionProgress(DeltaTime);
        ElapsedTime += DeltaTime;
        
        FProductionLine ProductionLine = ProductionSystem->GetProductionLine(ProductionLineID);
        if (ProductionLine.Status == EProductionStatus::Completed)
        {
            return true;
        }
        
        // 模擬延遲
        FPlatformProcess::Sleep(0.01f);
    }
    
    return false;
}

void UMingGoRTSProductionTest::SimulateTimeProgress(float DeltaTime)
{
    ProductionSystem->UpdateProductionProgress(DeltaTime);
}

bool UMingGoRTSProductionTest::VerifyResourceChanges(const TMap<EResourceType, float>& ExpectedChanges)
{
    // TODO: 實現資源變化驗證
    // 這需要與資源系統集成來檢查實際的資源變化
    return true;
}
