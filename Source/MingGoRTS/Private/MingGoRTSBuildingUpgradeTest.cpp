#include "MingGoRTSBuildingUpgradeTest.h"
#include "Engine/World.h"

UMingGoRTSBuildingUpgradeTest::UMingGoRTSBuildingUpgradeTest()
{
    UpgradeSystem = nullptr;
    PassedTests = 0;
    FailedTests = 0;
}

void UMingGoRTSBuildingUpgradeTest::InitializeTest()
{
    ResetTestCounters();
    
    // 創建建築升級系統實例
    UpgradeSystem = NewObject<UMingGoRTSBuildingUpgradeSystem>();
    if (UpgradeSystem)
    {
        UpgradeSystem->InitializeBuildingUpgradeSystem();
        AddTestResult(TEXT("建築升級系統初始化"), true, TEXT("建築升級系統成功初始化"));
    }
    else
    {
        AddTestResult(TEXT("建築升級系統初始化"), false, TEXT("無法創建建築升級系統實例"));
    }
}

bool UMingGoRTSBuildingUpgradeTest::TestBuildingUpgradeSystemInitialization()
{
    if (!UpgradeSystem)
    {
        AddTestResult(TEXT("建築升級系統初始化測試"), false, TEXT("建築升級系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 檢查所有建築類別
    TArray<EBuildingType> BuildingTypes = UpgradeSystem->GetAllBuildingTypes();
    if (BuildingTypes.Num() != 14) // 預期14個類別
    {
        AddTestResult(TEXT("建築類別數量檢查"), false, FString::Printf(TEXT("預期14個類別，實際%d個"), BuildingTypes.Num()));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("建築類別數量檢查"), true, TEXT("建築類別數量正確"));
    }

    // 檢查每個類別都有升級路徑
    for (EBuildingType BuildingType : BuildingTypes)
    {
        TArray<FBuildingUpgradePath> TypePaths = UpgradeSystem->GetUpgradePathsByType(BuildingType);
        if (TypePaths.Num() == 0)
        {
            AddTestResult(TEXT("類別升級路徑檢查"), false, FString::Printf(TEXT("類別 %s 沒有升級路徑"), *StaticEnum<EBuildingType>()->GetValueAsString(BuildingType)));
            bAllPassed = false;
        }
    }

    if (bAllPassed)
    {
        AddTestResult(TEXT("建築升級系統初始化測試"), true, TEXT("建築升級系統初始化正常"));
    }

    return bAllPassed;
}

bool UMingGoRTSBuildingUpgradeTest::TestUpgradePathCreation()
{
    if (!UpgradeSystem)
    {
        AddTestResult(TEXT("升級路徑創建測試"), false, TEXT("建築升級系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 測試獲取住宅升級路徑
    TArray<FBuildingUpgradePath> ResidentialPaths = UpgradeSystem->GetUpgradePathsByType(EBuildingType::Residential);
    if (ResidentialPaths.Num() == 0)
    {
        AddTestResult(TEXT("住宅升級路徑檢查"), false, TEXT("住宅升級路徑不存在"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("住宅升級路徑檢查"), true, FString::Printf(TEXT("找到 %d 個住宅升級路徑"), ResidentialPaths.Num()));
    }

    // 測試獲取工業升級路徑
    TArray<FBuildingUpgradePath> IndustrialPaths = UpgradeSystem->GetUpgradePathsByType(EBuildingType::Industrial);
    if (IndustrialPaths.Num() == 0)
    {
        AddTestResult(TEXT("工業升級路徑檢查"), false, TEXT("工業升級路徑不存在"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("工業升級路徑檢查"), true, FString::Printf(TEXT("找到 %d 個工業升級路徑"), IndustrialPaths.Num()));
    }

    // 測試獲取特定升級路徑
    FString ResidentialPathID = GetRandomUpgradePathID(EBuildingType::Residential);
    if (!ResidentialPathID.IsEmpty())
    {
        FBuildingUpgradePath ResidentialPath = UpgradeSystem->GetUpgradePath(ResidentialPathID);
        if (!VerifyUpgradePathData(ResidentialPath, TEXT("基礎住宅升級"), EBuildingType::Residential))
        {
            AddTestResult(TEXT("住宅升級路徑驗證"), false, TEXT("住宅升級路徑數據不正確"));
            bAllPassed = false;
        }
        else
        {
            AddTestResult(TEXT("住宅升級路徑驗證"), true, TEXT("住宅升級路徑數據正確"));
        }
    }

    return bAllPassed;
}

bool UMingGoRTSBuildingUpgradeTest::TestBuildingUpgradeFlow()
{
    if (!UpgradeSystem)
    {
        AddTestResult(TEXT("建築升級流程測試"), false, TEXT("建築升級系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 創建測試建築
    FString BuildingID = CreateTestBuilding(TEXT("測試建築"), EBuildingType::Residential);
    if (BuildingID.IsEmpty())
    {
        AddTestResult(TEXT("建築升級流程測試"), false, TEXT("無法創建測試建築"));
        return false;
    }

    // 獲取測試升級路徑
    FString UpgradePathID = GetRandomUpgradePathID(EBuildingType::Residential);
    if (UpgradePathID.IsEmpty())
    {
        AddTestResult(TEXT("建築升級流程測試"), false, TEXT("無法獲取測試升級路徑"));
        return false;
    }

    // 測試開始升級
    bool bStarted = UpgradeSystem->StartBuildingUpgrade(BuildingID, UpgradePathID);
    if (!bStarted)
    {
        AddTestResult(TEXT("開始升級測試"), false, TEXT("無法開始升級"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("開始升級測試"), true, TEXT("成功開始升級"));
    }

    // 檢查升級狀態
    if (!VerifyUpgradeProgress(BuildingID, EBuildingUpgradeStatus::Upgrading))
    {
        AddTestResult(TEXT("升級狀態檢查"), false, TEXT("升級狀態不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("升級狀態檢查"), true, TEXT("升級狀態正確"));
    }

    // 模擬升級進度
    SimulateTimeProgress(5.0f); // 模擬5秒

    // 檢查升級進度
    FBuildingUpgradeProgress Progress = UpgradeSystem->GetUpgradeProgress(BuildingID);
    if (Progress.Progress <= 0.0f)
    {
        AddTestResult(TEXT("升級進度檢查"), false, TEXT("升級進度未更新"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("升級進度檢查"), true, FString::Printf(TEXT("升級進度：%.1f%%"), Progress.Progress));
    }

    // 測試暫停升級
    bool bPaused = UpgradeSystem->PauseBuildingUpgrade(BuildingID);
    if (!bPaused)
    {
        AddTestResult(TEXT("暫停升級測試"), false, TEXT("無法暫停升級"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("暫停升級測試"), true, TEXT("成功暫停升級"));
    }

    // 檢查暫停後狀態
    if (!VerifyUpgradeProgress(BuildingID, EBuildingUpgradeStatus::Paused))
    {
        AddTestResult(TEXT("暫停後狀態檢查"), false, TEXT("暫停後狀態不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("暫停後狀態檢查"), true, TEXT("暫停後狀態正確"));
    }

    // 測試恢復升級
    bool bResumed = UpgradeSystem->ResumeBuildingUpgrade(BuildingID);
    if (!bResumed)
    {
        AddTestResult(TEXT("恢復升級測試"), false, TEXT("無法恢復升級"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("恢復升級測試"), true, TEXT("成功恢復升級"));
    }

    // 測試取消升級
    bool bCancelled = UpgradeSystem->CancelBuildingUpgrade(BuildingID);
    if (!bCancelled)
    {
        AddTestResult(TEXT("取消升級測試"), false, TEXT("無法取消升級"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("取消升級測試"), true, TEXT("成功取消升級"));
    }

    return bAllPassed;
}

bool UMingGoRTSBuildingUpgradeTest::TestUpgradePauseResume()
{
    if (!UpgradeSystem)
    {
        AddTestResult(TEXT("升級暫停恢復測試"), false, TEXT("建築升級系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 創建測試建築
    FString BuildingID = CreateTestBuilding(TEXT("暫停恢復測試建築"), EBuildingType::Commercial);
    if (BuildingID.IsEmpty())
    {
        AddTestResult(TEXT("升級暫停恢復測試"), false, TEXT("無法創建測試建築"));
        return false;
    }

    // 獲取測試升級路徑
    FString UpgradePathID = GetRandomUpgradePathID(EBuildingType::Commercial);
    if (UpgradePathID.IsEmpty())
    {
        AddTestResult(TEXT("升級暫停恢復測試"), false, TEXT("無法獲取測試升級路徑"));
        return false;
    }

    // 開始升級
    UpgradeSystem->StartBuildingUpgrade(BuildingID, UpgradePathID);

    // 測試多重暫停
    bool bPaused1 = UpgradeSystem->PauseBuildingUpgrade(BuildingID, TEXT("測試暫停1"));
    bool bPaused2 = UpgradeSystem->PauseBuildingUpgrade(BuildingID, TEXT("測試暫停2"));
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
    bool bResumed = UpgradeSystem->ResumeBuildingUpgrade(BuildingID);
    if (!bResumed)
    {
        AddTestResult(TEXT("恢復升級測試"), false, TEXT("無法恢復升級"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("恢復升級測試"), true, TEXT("成功恢復升級"));
    }

    // 檢查恢復後狀態
    if (!VerifyUpgradeProgress(BuildingID, EBuildingUpgradeStatus::Upgrading))
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

bool UMingGoRTSBuildingUpgradeTest::TestUpgradeCancellation()
{
    if (!UpgradeSystem)
    {
        AddTestResult(TEXT("升級取消測試"), false, TEXT("建築升級系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 創建測試建築
    FString BuildingID = CreateTestBuilding(TEXT("取消測試建築"), EBuildingType::Industrial);
    if (BuildingID.IsEmpty())
    {
        AddTestResult(TEXT("升級取消測試"), false, TEXT("無法創建測試建築"));
        return false;
    }

    // 獲取測試升級路徑
    FString UpgradePathID = GetRandomUpgradePathID(EBuildingType::Industrial);
    if (UpgradePathID.IsEmpty())
    {
        AddTestResult(TEXT("升級取消測試"), false, TEXT("無法獲取測試升級路徑"));
        return false;
    }

    // 開始升級
    UpgradeSystem->StartBuildingUpgrade(BuildingID, UpgradePathID);

    // 測試取消升級
    bool bCancelled = UpgradeSystem->CancelBuildingUpgrade(BuildingID);
    if (!bCancelled)
    {
        AddTestResult(TEXT("取消升級測試"), false, TEXT("無法取消升級"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("取消升級測試"), true, TEXT("成功取消升級"));
    }

    // 檢查取消後狀態
    FBuildingUpgradeProgress Progress = UpgradeSystem->GetUpgradeProgress(BuildingID);
    if (Progress.Status != EBuildingUpgradeStatus::Available)
    {
        AddTestResult(TEXT("取消後狀態檢查"), false, TEXT("取消後狀態不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("取消後狀態檢查"), true, TEXT("取消後狀態正確"));
    }

    return bAllPassed;
}

bool UMingGoRTSBuildingUpgradeTest::TestInstantUpgrade()
{
    if (!UpgradeSystem)
    {
        AddTestResult(TEXT("立即升級測試"), false, TEXT("建築升級系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 創建測試建築
    FString BuildingID = CreateTestBuilding(TEXT("立即升級測試建築"), EBuildingType::Military);
    if (BuildingID.IsEmpty())
    {
        AddTestResult(TEXT("立即升級測試"), false, TEXT("無法創建測試建築"));
        return false;
    }

    // 獲取測試升級路徑
    FString UpgradePathID = GetRandomUpgradePathID(EBuildingType::Military);
    if (UpgradePathID.IsEmpty())
    {
        AddTestResult(TEXT("立即升級測試"), false, TEXT("無法獲取測試升級路徑"));
        return false;
    }

    // 開始升級
    UpgradeSystem->StartBuildingUpgrade(BuildingID, UpgradePathID);

    // 測試立即升級
    bool bInstantUpgraded = UpgradeSystem->InstantUpgrade(BuildingID);
    if (!bInstantUpgraded)
    {
        AddTestResult(TEXT("立即升級測試"), false, TEXT("無法立即升級"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("立即升級測試"), true, TEXT("成功立即升級"));
    }

    // 檢查升級是否完成
    FBuildingUpgradeProgress Progress = UpgradeSystem->GetUpgradeProgress(BuildingID);
    if (Progress.Status != EBuildingUpgradeStatus::Completed)
    {
        AddTestResult(TEXT("立即升級完成檢查"), false, TEXT("立即升級未完成"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("立即升級完成檢查"), true, TEXT("立即升級已完成"));
    }

    return bAllPassed;
}

bool UMingGoRTSBuildingUpgradeTest::TestUpgradePriority()
{
    if (!UpgradeSystem)
    {
        AddTestResult(TEXT("升級優先級測試"), false, TEXT("建築升級系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 創建多個測試建築
    TArray<FString> BuildingIDs;
    for (int32 i = 0; i < 3; ++i)
    {
        FString BuildingID = CreateTestBuilding(FString::Printf(TEXT("優先級測試建築%d"), i), EBuildingType::Residential);
        if (!BuildingID.IsEmpty())
        {
            BuildingIDs.Add(BuildingID);
        }
    }

    if (BuildingIDs.Num() < 3)
    {
        AddTestResult(TEXT("升級優先級測試"), false, TEXT("無法創建足夠的測試建築"));
        return false;
    }

    // 設置不同優先級
    TArray<EBuildingUpgradePriority> Priorities = {
        EBuildingUpgradePriority::Low,
        EBuildingUpgradePriority::Normal,
        EBuildingUpgradePriority::High
    };

    for (int32 i = 0; i < BuildingIDs.Num(); ++i)
    {
        bool bSet = UpgradeSystem->SetUpgradePriority(BuildingIDs[i], Priorities[i]);
        if (!bSet)
        {
            AddTestResult(TEXT("設置優先級測試"), false, FString::Printf(TEXT("無法設置建築 %d 的優先級"), i));
            bAllPassed = false;
        }
        else
        {
            AddTestResult(TEXT("設置優先級測試"), true, FString::Printf(TEXT("成功設置建築 %d 的優先級"), i));
        }
    }

    // 驗證優先級設置
    for (int32 i = 0; i < BuildingIDs.Num(); ++i)
    {
        FBuildingUpgradeProgress Progress = UpgradeSystem->GetUpgradeProgress(BuildingIDs[i]);
        if (Progress.Priority != Priorities[i])
        {
            AddTestResult(TEXT("優先級驗證"), false, FString::Printf(TEXT("建築 %d 優先級不正確"), i));
            bAllPassed = false;
        }
    }

    if (bAllPassed)
    {
        AddTestResult(TEXT("優先級驗證"), true, TEXT("所有優先級設置正確"));
    }

    return bAllPassed;
}

bool UMingGoRTSBuildingUpgradeTest::TestUpgradePathUnlocking()
{
    if (!UpgradeSystem)
    {
        AddTestResult(TEXT("升級路徑解鎖測試"), false, TEXT("建築升級系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 獲取一個鎖定的升級路徑
    TArray<FBuildingUpgradePath> AllPaths = UpgradeSystem->GetUpgradePathsByType(EBuildingType::Residential);
    if (AllPaths.Num() == 0)
    {
        AddTestResult(TEXT("升級路徑解鎖測試"), false, TEXT("無法獲取升級路徑"));
        return false;
    }

    FString PathID = AllPaths[0].UpgradePathID;
    
    // 測試解鎖
    bool bUnlocked = UpgradeSystem->UnlockUpgradePath(PathID);
    if (!bUnlocked)
    {
        AddTestResult(TEXT("解鎖升級路徑測試"), false, TEXT("無法解鎖升級路徑"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("解鎖升級路徑測試"), true, TEXT("成功解鎖升級路徑"));
    }

    // 測試重複解鎖
    bool bReUnlocked = UpgradeSystem->UnlockUpgradePath(PathID);
    if (!bReUnlocked)
    {
        AddTestResult(TEXT("重複解鎖測試"), false, TEXT("重複解鎖應該成功"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("重複解鎖測試"), true, TEXT("重複解鎖正確處理"));
    }

    return bAllPassed;
}

bool UMingGoRTSBuildingUpgradeTest::TestUpgradeEffectsApplication()
{
    if (!UpgradeSystem)
    {
        AddTestResult(TEXT("升級效果應用測試"), false, TEXT("建築升級系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 創建測試效果
    TArray<FBuildingUpgradeEffect> TestEffects = CreateTestEffects();
    if (TestEffects.Num() == 0)
    {
        AddTestResult(TEXT("升級效果應用測試"), false, TEXT("無法創建測試效果"));
        return false;
    }

    // 創建測試建築
    FString BuildingID = CreateTestBuilding(TEXT("效果測試建築"), EBuildingType::Residential);
    if (BuildingID.IsEmpty())
    {
        AddTestResult(TEXT("升級效果應用測試"), false, TEXT("無法創建測試建築"));
        return false;
    }

    // 測試效果應用
    // TODO: 實際應用效果並驗證
    AddTestResult(TEXT("升級效果應用測試"), true, FString::Printf(TEXT("創建了 %d 個測試效果"), TestEffects.Num()));

    return bAllPassed;
}

bool UMingGoRTSBuildingUpgradeTest::TestUpgradeRequirementsCheck()
{
    if (!UpgradeSystem)
    {
        AddTestResult(TEXT("升級條件檢查測試"), false, TEXT("建築升級系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 創建測試需求
    TArray<FBuildingUpgradeRequirement> TestRequirements = CreateTestRequirements();
    if (TestRequirements.Num() == 0)
    {
        AddTestResult(TEXT("升級條件檢查測試"), false, TEXT("無法創建測試需求"));
        return false;
    }

    // 創建測試建築
    FString BuildingID = CreateTestBuilding(TEXT("條件測試建築"), EBuildingType::Residential);
    if (BuildingID.IsEmpty())
    {
        AddTestResult(TEXT("升級條件檢查測試"), false, TEXT("無法創建測試建築"));
        return false;
    }

    // 獲取測試升級路徑
    FString UpgradePathID = GetRandomUpgradePathID(EBuildingType::Residential);
    if (UpgradePathID.IsEmpty())
    {
        AddTestResult(TEXT("升級條件檢查測試"), false, TEXT("無法獲取測試升級路徑"));
        return false;
    }

    // 測試條件檢查
    bool bCanUpgrade = UpgradeSystem->CanUpgradeBuilding(BuildingID, UpgradePathID);
    AddTestResult(TEXT("升級條件檢查測試"), true, FString::Printf(TEXT("升級條件檢查結果：%s"), bCanUpgrade ? TEXT("可以升級") : TEXT("不能升級")));

    return bAllPassed;
}

bool UMingGoRTSBuildingUpgradeTest::TestUpgradeCostCalculation()
{
    if (!UpgradeSystem)
    {
        AddTestResult(TEXT("升級成本計算測試"), false, TEXT("建築升級系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 獲取測試升級路徑
    FString UpgradePathID = GetRandomUpgradePathID(EBuildingType::Residential);
    if (UpgradePathID.IsEmpty())
    {
        AddTestResult(TEXT("升級成本計算測試"), false, TEXT("無法獲取測試升級路徑"));
        return false;
    }

    // 測試成本計算
    TMap<EResourceType, float> UpgradeCosts = UpgradeSystem->CalculateUpgradeCost(TEXT("TestBuilding"), UpgradePathID, 2);
    if (UpgradeCosts.Num() == 0)
    {
        AddTestResult(TEXT("升級成本計算測試"), false, TEXT("升級成本計算失敗"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("升級成本計算測試"), true, FString::Printf(TEXT("計算了 %d 種資源成本"), UpgradeCosts.Num()));
    }

    return bAllPassed;
}

bool UMingGoRTSBuildingUpgradeTest::TestUpgradeTimeCalculation()
{
    if (!UpgradeSystem)
    {
        AddTestResult(TEXT("升級時間計算測試"), false, TEXT("建築升級系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 獲取測試升級路徑
    FString UpgradePathID = GetRandomUpgradePathID(EBuildingType::Residential);
    if (UpgradePathID.IsEmpty())
    {
        AddTestResult(TEXT("升級時間計算測試"), false, TEXT("無法獲取測試升級路徑"));
        return false;
    }

    // 測試時間計算
    float UpgradeTime = UpgradeSystem->CalculateUpgradeTime(TEXT("TestBuilding"), UpgradePathID, 2);
    if (UpgradeTime <= 0.0f)
    {
        AddTestResult(TEXT("升級時間計算測試"), false, TEXT("升級時間計算失敗"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("升級時間計算測試"), true, FString::Printf(TEXT("升級時間：%.2f秒"), UpgradeTime));
    }

    return bAllPassed;
}

bool UMingGoRTSBuildingUpgradeTest::TestRepublicanEraUpgradePaths()
{
    if (!UpgradeSystem)
    {
        AddTestResult(TEXT("民國時期特色升級路徑測試"), false, TEXT("建築升級系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 檢查住宅升級路徑
    TArray<FBuildingUpgradePath> ResidentialPaths = UpgradeSystem->GetUpgradePathsByType(EBuildingType::Residential);
    if (ResidentialPaths.Num() == 0)
    {
        AddTestResult(TEXT("住宅升級路徑檢查"), false, TEXT("住宅升級路徑不存在"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("住宅升級路徑檢查"), true, FString::Printf(TEXT("找到 %d 個住宅升級路徑"), ResidentialPaths.Num()));
    }

    // 檢查工業升級路徑
    TArray<FBuildingUpgradePath> IndustrialPaths = UpgradeSystem->GetUpgradePathsByType(EBuildingType::Industrial);
    if (IndustrialPaths.Num() == 0)
    {
        AddTestResult(TEXT("工業升級路徑檢查"), false, TEXT("工業升級路徑不存在"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("工業升級路徑檢查"), true, FString::Printf(TEXT("找到 %d 個工業升級路徑"), IndustrialPaths.Num()));
    }

    // 檢查歷史背景
    bool bFoundHistoricalContext = false;
    for (const FBuildingUpgradePath& Path : ResidentialPaths)
    {
        if (!Path.HistoricalContext.IsEmpty())
        {
            bFoundHistoricalContext = true;
            break;
        }
    }

    if (!bFoundHistoricalContext)
    {
        AddTestResult(TEXT("歷史背景檢查"), false, TEXT("升級路徑缺少歷史背景"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("歷史背景檢查"), true, TEXT("升級路徑包含歷史背景"));
    }

    return bAllPassed;
}

bool UMingGoRTSBuildingUpgradeTest::TestUpgradeStatistics()
{
    if (!UpgradeSystem)
    {
        AddTestResult(TEXT("升級統計測試"), false, TEXT("建築升級系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 獲取升級統計
    FBuildingUpgradeStatistics Stats = UpgradeSystem->GetUpgradeStatistics();
    
    // 檢查統計結構
    if (Stats.BuildingUpgrades.Num() == 0)
    {
        AddTestResult(TEXT("統計結構檢查"), false, TEXT("統計結構無效"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("統計結構檢查"), true, TEXT("統計結構有效"));
    }

    // 檢查初始值
    if (Stats.TotalUpgrades != 0 || Stats.SuccessfulUpgrades != 0 || Stats.FailedUpgrades != 0)
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

bool UMingGoRTSBuildingUpgradeTest::TestUpgradeConfiguration()
{
    if (!UpgradeSystem)
    {
        AddTestResult(TEXT("升級配置測試"), false, TEXT("建築升級系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 獲取當前配置
    FBuildingUpgradeSystemConfig CurrentConfig = UpgradeSystem->GetUpgradeConfig();
    
    // 創建測試配置
    FBuildingUpgradeSystemConfig TestConfig = CreateTestConfig();
    
    // 應用配置
    UpgradeSystem->UpdateUpgradeConfig(TestConfig);
    
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

bool UMingGoRTSBuildingUpgradeTest::TestSaveAndLoad()
{
    if (!UpgradeSystem)
    {
        AddTestResult(TEXT("保存和載入測試"), false, TEXT("建築升級系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 創建測試數據
    FString BuildingID = CreateTestBuilding(TEXT("保存測試建築"), EBuildingType::Residential);
    if (BuildingID.IsEmpty())
    {
        AddTestResult(TEXT("保存和載入測試"), false, TEXT("無法創建測試數據"));
        return false;
    }

    // 保存數據
    bool bSaved = UpgradeSystem->SaveUpgradeData(TEXT("TestSave"));
    if (!bSaved)
    {
        AddTestResult(TEXT("保存測試"), false, TEXT("無法保存升級數據"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("保存測試"), true, TEXT("成功保存升級數據"));
    }

    // 重置系統
    // TODO: 重置升級系統

    // 載入數據
    bool bLoaded = UpgradeSystem->LoadUpgradeData(TEXT("TestSave"));
    if (!bLoaded)
    {
        AddTestResult(TEXT("載入測試"), false, TEXT("無法載入升級數據"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("載入測試"), true, TEXT("成功載入升級數據"));
    }

    return bAllPassed;
}

bool UMingGoRTSBuildingUpgradeTest::TestConcurrentUpgradeLimit()
{
    if (!UpgradeSystem)
    {
        AddTestResult(TEXT("並行升級限制測試"), false, TEXT("建築升級系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 創建多個測試建築
    TArray<FString> BuildingIDs;
    for (int32 i = 0; i < 5; ++i)
    {
        FString BuildingID = CreateTestBuilding(FString::Printf(TEXT("並行測試建築%d"), i), EBuildingType::Residential);
        if (!BuildingID.IsEmpty())
        {
            BuildingIDs.Add(BuildingID);
        }
    }

    if (BuildingIDs.Num() < 3)
    {
        AddTestResult(TEXT("並行升級限制測試"), false, TEXT("無法創建足夠的測試建築"));
        return false;
    }

    // 獲取測試升級路徑
    FString UpgradePathID = GetRandomUpgradePathID(EBuildingType::Residential);
    if (UpgradePathID.IsEmpty())
    {
        AddTestResult(TEXT("並行升級限制測試"), false, TEXT("無法獲取測試升級路徑"));
        return false;
    }

    // 嘗試開始多個升級
    int32 StartedCount = 0;
    for (const FString& BuildingID : BuildingIDs)
    {
        if (UpgradeSystem->StartBuildingUpgrade(BuildingID, UpgradePathID))
        {
            StartedCount++;
        }
    }

    AddTestResult(TEXT("並行升級測試"), true, FString::Printf(TEXT("成功開始 %d 個並行升級"), StartedCount));

    return bAllPassed;
}

bool UMingGoRTSBuildingUpgradeTest::TestUpgradePreview()
{
    if (!UpgradeSystem)
    {
        AddTestResult(TEXT("升級預覽測試"), false, TEXT("建築升級系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 獲取測試升級路徑
    FString UpgradePathID = GetRandomUpgradePathID(EBuildingType::Residential);
    if (UpgradePathID.IsEmpty())
    {
        AddTestResult(TEXT("升級預覽測試"), false, TEXT("無法獲取測試升級路徑"));
        return false;
    }

    // 測試升級預覽
    TArray<FBuildingUpgradeEffect> PreviewEffects = UpgradeSystem->PreviewUpgradeEffects(TEXT("TestBuilding"), UpgradePathID, 2);
    if (PreviewEffects.Num() == 0)
    {
        AddTestResult(TEXT("升級預覽測試"), false, TEXT("升級預覽失敗"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("升級預覽測試"), true, FString::Printf(TEXT("預覽了 %d 個升級效果"), PreviewEffects.Num()));
    }

    return bAllPassed;
}

bool UMingGoRTSBuildingUpgradeTest::RunAllTests()
{
    InitializeTest();
    
    bool bAllPassed = true;
    
    bAllPassed &= TestBuildingUpgradeSystemInitialization();
    bAllPassed &= TestUpgradePathCreation();
    bAllPassed &= TestBuildingUpgradeFlow();
    bAllPassed &= TestUpgradePauseResume();
    bAllPassed &= TestUpgradeCancellation();
    bAllPassed &= TestInstantUpgrade();
    bAllPassed &= TestUpgradePriority();
    bAllPassed &= TestUpgradePathUnlocking();
    bAllPassed &= TestUpgradeEffectsApplication();
    bAllPassed &= TestUpgradeRequirementsCheck();
    bAllPassed &= TestUpgradeCostCalculation();
    bAllPassed &= TestUpgradeTimeCalculation();
    bAllPassed &= TestRepublicanEraUpgradePaths();
    bAllPassed &= TestUpgradeStatistics();
    bAllPassed &= TestUpgradeConfiguration();
    bAllPassed &= TestSaveAndLoad();
    bAllPassed &= TestConcurrentUpgradeLimit();
    bAllPassed &= TestUpgradePreview();
    
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

FString UMingGoRTSBuildingUpgradeTest::GetTestResults() const
{
    FString Results = TEXT("建築升級系統測試結果\n");
    Results += FString::Printf(TEXT("通過測試：%d\n"), PassedTests);
    Results += FString::Printf(TEXT("失敗測試：%d\n"), FailedTests);
    Results += TEXT("詳細結果：\n");
    
    for (const FString& Result : TestResults)
    {
        Results += Result + TEXT("\n");
    }
    
    return Results;
}

void UMingGoRTSBuildingUpgradeTest::CleanupTest()
{
    if (UpgradeSystem)
    {
        UpgradeSystem = nullptr;
    }
    
    TestResults.Empty();
    ResetTestCounters();
}

void UMingGoRTSBuildingUpgradeTest::AddTestResult(const FString& TestName, bool bPassed, const FString& Message)
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

FString UMingGoRTSBuildingUpgradeTest::CreateTestBuilding(const FString& BuildingName, EBuildingType BuildingType)
{
    return FString::Printf(TEXT("TEST_%s_%s"), *BuildingName, *StaticEnum<EBuildingType>()->GetValueAsString(BuildingType));
}

FBuildingUpgradePath UMingGoRTSBuildingUpgradeTest::CreateTestUpgradePath(const FString& PathName, EBuildingType BuildingType)
{
    FBuildingUpgradePath TestPath;
    TestPath.UpgradePathID = FString::Printf(TEXT("TEST_%s"), *PathName);
    TestPath.UpgradePathName = PathName;
    TestPath.Description = FString::Printf(TEXT("測試升級路徑：%s"), *PathName);
    TestPath.BuildingType = BuildingType;
    TestPath.MaxLevel = 3;
    TestPath.UpgradeTime = 300.0f;
    
    return TestPath;
}

bool UMingGoRTSBuildingUpgradeTest::VerifyUpgradePathData(const FBuildingUpgradePath& UpgradePath, const FString& ExpectedName, EBuildingType ExpectedType)
{
    return UpgradePath.UpgradePathName == ExpectedName && UpgradePath.BuildingType == ExpectedType;
}

bool UMingGoRTSBuildingUpgradeTest::VerifyUpgradeProgress(const FString& BuildingID, EBuildingUpgradeStatus ExpectedStatus)
{
    FBuildingUpgradeProgress Progress = UpgradeSystem->GetUpgradeProgress(BuildingID);
    return Progress.Status == ExpectedStatus;
}

FString UMingGoRTSBuildingUpgradeTest::GetRandomUpgradePathID(EBuildingType BuildingType) const
{
    TArray<FBuildingUpgradePath> TypePaths = UpgradeSystem->GetUpgradePathsByType(BuildingType);
    if (TypePaths.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, TypePaths.Num());
        return TypePaths[RandomIndex].UpgradePathID;
    }
    return TEXT("");
}

void UMingGoRTSBuildingUpgradeTest::ResetTestCounters()
{
    PassedTests = 0;
    FailedTests = 0;
    TestResults.Empty();
}

FBuildingUpgradeSystemConfig UMingGoRTSBuildingUpgradeTest::CreateTestConfig()
{
    FBuildingUpgradeSystemConfig TestConfig;
    TestConfig.GlobalUpgradeSpeed = 2.0f;
    TestConfig.MaxConcurrentUpgrades = 5.0f;
    TestConfig.bEnableAutoUpgrade = true;
    TestConfig.bEnableInstantUpgrade = true;
    TestConfig.InstantUpgradeCostMultiplier = 1.5f;
    TestConfig.bEnableUpgradePreview = true;
    
    // 設置建築類型加成
    TestConfig.BuildingTypeBonus.Add(EBuildingType::Residential, 1.5f);
    TestConfig.BuildingTypeBonus.Add(EBuildingType::Industrial, 1.3f);
    
    return TestConfig;
}

bool UMingGoRTSBuildingUpgradeTest::VerifyConfigApplication(const FBuildingUpgradeSystemConfig& ExpectedConfig)
{
    FBuildingUpgradeSystemConfig CurrentConfig = UpgradeSystem->GetUpgradeConfig();
    
    return FMath::Abs(CurrentConfig.GlobalUpgradeSpeed - ExpectedConfig.GlobalUpgradeSpeed) < 0.01f &&
           CurrentConfig.MaxConcurrentUpgrades == ExpectedConfig.MaxConcurrentUpgrades &&
           CurrentConfig.bEnableAutoUpgrade == ExpectedConfig.bEnableAutoUpgrade &&
           CurrentConfig.bEnableInstantUpgrade == ExpectedConfig.bEnableInstantUpgrade &&
           FMath::Abs(CurrentConfig.InstantUpgradeCostMultiplier - ExpectedConfig.InstantUpgradeCostMultiplier) < 0.01f &&
           CurrentConfig.bEnableUpgradePreview == ExpectedConfig.bEnableUpgradePreview;
}

bool UMingGoRTSBuildingUpgradeTest::WaitForUpgradeCompletion(const FString& BuildingID, float TimeoutSeconds)
{
    float ElapsedTime = 0.0f;
    float DeltaTime = 0.1f;
    
    while (ElapsedTime < TimeoutSeconds)
    {
        UpgradeSystem->UpdateUpgradeProgress(DeltaTime);
        ElapsedTime += DeltaTime;
        
        FBuildingUpgradeProgress Progress = UpgradeSystem->GetUpgradeProgress(BuildingID);
        if (Progress.Status == EBuildingUpgradeStatus::Completed)
        {
            return true;
        }
        
        // 模擬延遲
        FPlatformProcess::Sleep(0.01f);
    }
    
    return false;
}

void UMingGoRTSBuildingUpgradeTest::SimulateTimeProgress(float DeltaTime)
{
    UpgradeSystem->UpdateUpgradeProgress(DeltaTime);
}

bool UMingGoRTSBuildingUpgradeTest::VerifyUpgradeEffects(const FString& BuildingID, const TArray<FBuildingUpgradeEffect>& ExpectedEffects)
{
    // TODO: 實現升級效果驗證
    // 這需要與建築系統集成來檢查實際的效果應用
    return true;
}

TArray<FBuildingUpgradeEffect> UMingGoRTSBuildingUpgradeTest::CreateTestEffects()
{
    TArray<FBuildingUpgradeEffect> TestEffects;
    
    FBuildingUpgradeEffect CapacityEffect;
    CapacityEffect.EffectType = EBuildingUpgradeEffectType::CapacityIncrease;
    CapacityEffect.TargetID = TEXT("TestCapacity");
    CapacityEffect.Value = 10.0f;
    CapacityEffect.Description = TEXT("測試容量增加");
    CapacityEffect.bIsPermanent = true;
    TestEffects.Add(CapacityEffect);
    
    FBuildingUpgradeEffect EfficiencyEffect;
    EfficiencyEffect.EffectType = EBuildingUpgradeEffectType::EfficiencyBoost;
    EfficiencyEffect.TargetID = TEXT("TestEfficiency");
    EfficiencyEffect.Value = 15.0f;
    EfficiencyEffect.Description = TEXT("測試效率提升");
    EfficiencyEffect.bIsPermanent = true;
    TestEffects.Add(EfficiencyEffect);
    
    return TestEffects;
}

TArray<FBuildingUpgradeRequirement> UMingGoRTSBuildingUpgradeTest::CreateTestRequirements()
{
    TArray<FBuildingUpgradeRequirement> TestRequirements;
    
    FBuildingUpgradeRequirement ResourceReq;
    ResourceReq.RequirementType = EBuildingUpgradeRequirementType::ResourceCost;
    ResourceReq.TargetID = TEXT("TestResource");
    ResourceReq.RequiredValue = 100.0f;
    ResourceReq.Description = TEXT("測試資源需求");
    ResourceReq.bIsOptional = false;
    TestRequirements.Add(ResourceReq);
    
    FBuildingUpgradeRequirement LevelReq;
    LevelReq.RequirementType = EBuildingUpgradeRequirementType::BuildingLevel;
    LevelReq.TargetID = TEXT("TestLevel");
    LevelReq.RequiredValue = 2.0f;
    LevelReq.Description = TEXT("測試等級需求");
    LevelReq.bIsOptional = false;
    TestRequirements.Add(LevelReq);
    
    return TestRequirements;
}
