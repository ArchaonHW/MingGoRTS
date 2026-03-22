#include "MingGoRTSResourceCollectionTest.h"
#include "Engine/World.h"

UMingGoRTSResourceCollectionTest::UMingGoRTSResourceCollectionTest()
{
    ResourceSystem = nullptr;
    PassedTests = 0;
    FailedTests = 0;
}

void UMingGoRTSResourceCollectionTest::InitializeTest()
{
    ResetTestCounters();
    
    // 創建資源系統實例
    ResourceSystem = NewObject<UMingGoRTSResourceSystem>();
    if (ResourceSystem)
    {
        ResourceSystem->InitializeResourceSystem();
        AddTestResult(TEXT("資源系統初始化"), true, TEXT("資源系統成功初始化"));
    }
    else
    {
        AddTestResult(TEXT("資源系統初始化"), false, TEXT("無法創建資源系統實例"));
    }
}

bool UMingGoRTSResourceCollectionTest::TestBasicResourceCollection()
{
    if (!ResourceSystem)
    {
        AddTestResult(TEXT("基本資源收集測試"), false, TEXT("資源系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 測試創建資源收集
    FString CollectionID = CreateTestCollection(EResourceType::Wood, ECollectionMethod::Harvest);
    if (CollectionID.IsEmpty())
    {
        AddTestResult(TEXT("創建資源收集"), false, TEXT("無法創建資源收集"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("創建資源收集"), true, FString::Printf(TEXT("成功創建收集：%s"), *CollectionID));
    }

    // 測試開始收集
    bool bStarted = ResourceSystem->StartResourceCollection(CollectionID, TEXT("TestCollector"));
    if (!bStarted)
    {
        AddTestResult(TEXT("開始資源收集"), false, TEXT("無法開始資源收集"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("開始資源收集"), true, TEXT("成功開始資源收集"));
    }

    // 測試收集資源
    float CollectedAmount = ResourceSystem->CollectResource(CollectionID);
    if (CollectedAmount <= 0.0f)
    {
        AddTestResult(TEXT("收集資源"), false, TEXT("收集資源失敗"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("收集資源"), true, FString::Printf(TEXT("成功收集：%.2f"), CollectedAmount));
    }

    // 測試停止收集
    bool bStopped = ResourceSystem->StopResourceCollection(CollectionID);
    if (!bStopped)
    {
        AddTestResult(TEXT("停止資源收集"), false, TEXT("無法停止資源收集"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("停止資源收集"), true, TEXT("成功停止資源收集"));
    }

    return bAllPassed;
}

bool UMingGoRTSResourceCollectionTest::TestResourceStorage()
{
    if (!ResourceSystem)
    {
        AddTestResult(TEXT("資源存儲測試"), false, TEXT("資源系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 創建測試存儲
    FString StorageID = CreateTestStorage(TEXT("TestStorage"), 1000.0f);
    if (StorageID.IsEmpty())
    {
        AddTestResult(TEXT("創建資源存儲"), false, TEXT("無法創建資源存儲"));
        return false;
    }

    // 測試存儲資源
    bool bStored = ResourceSystem->StoreResource(StorageID, EResourceType::Wood, 100.0f);
    if (!bStored)
    {
        AddTestResult(TEXT("存儲資源"), false, TEXT("無法存儲資源"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("存儲資源"), true, TEXT("成功存儲資源"));
    }

    // 驗證資源數量
    bool bVerified = VerifyResourceQuantity(StorageID, EResourceType::Wood, 100.0f);
    if (!bVerified)
    {
        AddTestResult(TEXT("驗證資源數量"), false, TEXT("資源數量不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("驗證資源數量"), true, TEXT("資源數量正確"));
    }

    // 測試提取資源
    float RetrievedAmount = ResourceSystem->RetrieveResource(StorageID, EResourceType::Wood, 50.0f);
    if (RetrievedAmount <= 0.0f)
    {
        AddTestResult(TEXT("提取資源"), false, TEXT("無法提取資源"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("提取資源"), true, FString::Printf(TEXT("成功提取：%.2f"), RetrievedAmount));
    }

    // 驗證剩餘資源
    bool bRemainingVerified = VerifyResourceQuantity(StorageID, EResourceType::Wood, 50.0f);
    if (!bRemainingVerified)
    {
        AddTestResult(TEXT("驗證剩餘資源"), false, TEXT("剩餘資源數量不正確"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("驗證剩餘資源"), true, TEXT("剩餘資源數量正確"));
    }

    return bAllPassed;
}

bool UMingGoRTSResourceCollectionTest::TestResourceConversion()
{
    if (!ResourceSystem)
    {
        AddTestResult(TEXT("資源轉換測試"), false, TEXT("資源系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 創建測試存儲並添加輸入資源
    FString StorageID = CreateTestStorage(TEXT("ConversionStorage"), 2000.0f);
    if (StorageID.IsEmpty())
    {
        AddTestResult(TEXT("資源轉換測試"), false, TEXT("無法創建轉換測試存儲"));
        return false;
    }

    // 添加輸入資源
    ResourceSystem->StoreResource(StorageID, EResourceType::Wood, 100.0f);
    ResourceSystem->StoreResource(StorageID, EResourceType::Metal, 50.0f);

    // 創建轉換配方
    TMap<EResourceType, float> InputResources;
    InputResources.Add(EResourceType::Wood, 50.0f);
    InputResources.Add(EResourceType::Metal, 25.0f);

    TMap<EResourceType, float> OutputResources;
    OutputResources.Add(EResourceType::Tools, 10.0f);

    FString ConversionID = ResourceSystem->CreateResourceConversion(TEXT("TestConversion"), InputResources, OutputResources);
    if (ConversionID.IsEmpty())
    {
        AddTestResult(TEXT("創建資源轉換"), false, TEXT("無法創建資源轉換"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("創建資源轉換"), true, FString::Printf(TEXT("成功創建轉換：%s"), *ConversionID));
    }

    // 執行轉換
    bool bConverted = ResourceSystem->ConvertResources(ConversionID);
    if (!bConverted)
    {
        AddTestResult(TEXT("執行資源轉換"), false, TEXT("無法執行資源轉換"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("執行資源轉換"), true, TEXT("成功執行資源轉換"));
    }

    return bAllPassed;
}

bool UMingGoRTSResourceCollectionTest::TestResourceTransport()
{
    if (!ResourceSystem)
    {
        AddTestResult(TEXT("資源運輸測試"), false, TEXT("資源系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 創建源存儲和目標存儲
    FString FromStorageID = CreateTestStorage(TEXT("FromStorage"), 1000.0f);
    FString ToStorageID = CreateTestStorage(TEXT("ToStorage"), 1000.0f);

    if (FromStorageID.IsEmpty() || ToStorageID.IsEmpty())
    {
        AddTestResult(TEXT("資源運輸測試"), false, TEXT("無法創建運輸測試存儲"));
        return false;
    }

    // 在源存儲中添加資源
    ResourceSystem->StoreResource(FromStorageID, EResourceType::Wood, 100.0f);

    // 測試資源運輸
    bool bTransported = ResourceSystem->TransportResource(FromStorageID, ToStorageID, EResourceType::Wood, 50.0f);
    if (!bTransported)
    {
        AddTestResult(TEXT("資源運輸"), false, TEXT("無法運輸資源"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("資源運輸"), true, TEXT("成功運輸資源"));
    }

    // 驗證運輸結果
    bool bFromVerified = VerifyResourceQuantity(FromStorageID, EResourceType::Wood, 50.0f);
    bool bToVerified = VerifyResourceQuantity(ToStorageID, EResourceType::Wood, 50.0f);

    if (!bFromVerified || !bToVerified)
    {
        AddTestResult(TEXT("驗證運輸結果"), false, TEXT("運輸結果驗證失敗"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("驗證運輸結果"), true, TEXT("運輸結果驗證成功"));
    }

    return bAllPassed;
}

bool UMingGoRTSResourceCollectionTest::TestResourceTrade()
{
    if (!ResourceSystem)
    {
        AddTestResult(TEXT("資源交易測試"), false, TEXT("資源系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 創建測試存儲
    FString StorageID = CreateTestStorage(TEXT("TradeStorage"), 2000.0f);
    if (StorageID.IsEmpty())
    {
        AddTestResult(TEXT("資源交易測試"), false, TEXT("無法創建交易測試存儲"));
        return false;
    }

    // 添加交易資源
    ResourceSystem->StoreResource(StorageID, EResourceType::Wood, 100.0f);

    // 測試資源交易
    bool bTraded = ResourceSystem->TradeResource(StorageID, EResourceType::Wood, 50.0f, EResourceType::Stone, 75.0f);
    if (!bTraded)
    {
        AddTestResult(TEXT("資源交易"), false, TEXT("無法執行資源交易"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("資源交易"), true, TEXT("成功執行資源交易"));
    }

    return bAllPassed;
}

bool UMingGoRTSResourceCollectionTest::TestResourceDecay()
{
    if (!ResourceSystem)
    {
        AddTestResult(TEXT("資源衰減測試"), false, TEXT("資源系統未初始化"));
        return false;
    }

    bool bAllPassed = true;

    // 創建測試存儲
    FString StorageID = CreateTestStorage(TEXT("DecayStorage"), 1000.0f);
    if (StorageID.IsEmpty())
    {
        AddTestResult(TEXT("資源衰減測試"), false, TEXT("無法創建衰減測試存儲"));
        return false;
    }

    // 添加易腐爛資源
    ResourceSystem->StoreResource(StorageID, EResourceType::Food, 100.0f);

    // 獲取初始數量
    FResourceData InitialResource = ResourceSystem->GetStoredResource(StorageID, EResourceType::Food);
    float InitialQuantity = InitialResource.Quantity;

    // 模擬時間流逝（1小時）
    ResourceSystem->ProcessResourceDecay(3600.0f);

    // 獲取衰減後數量
    FResourceData FinalResource = ResourceSystem->GetStoredResource(StorageID, EResourceType::Food);
    float FinalQuantity = FinalResource.Quantity;

    // 檢查是否有衰減
    if (FinalQuantity >= InitialQuantity)
    {
        AddTestResult(TEXT("資源衰減"), false, TEXT("資源未發生衰減"));
        bAllPassed = false;
    }
    else
    {
        AddTestResult(TEXT("資源衰減"), true, FString::Printf(TEXT("資源從 %.2f 衰減到 %.2f"), InitialQuantity, FinalQuantity));
    }

    return bAllPassed;
}

bool UMingGoRTSResourceCollectionTest::RunAllTests()
{
    InitializeTest();
    
    bool bAllPassed = true;
    
    bAllPassed &= TestBasicResourceCollection();
    bAllPassed &= TestResourceStorage();
    bAllPassed &= TestResourceConversion();
    bAllPassed &= TestResourceTransport();
    bAllPassed &= TestResourceTrade();
    bAllPassed &= TestResourceDecay();
    
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

FString UMingGoRTSResourceCollectionTest::GetTestResults() const
{
    FString Results = TEXT("資源收集系統測試結果\n");
    Results += FString::Printf(TEXT("通過測試：%d\n"), PassedTests);
    Results += FString::Printf(TEXT("失敗測試：%d\n"), FailedTests);
    Results += TEXT("詳細結果：\n");
    
    for (const FString& Result : TestResults)
    {
        Results += Result + TEXT("\n");
    }
    
    return Results;
}

void UMingGoRTSResourceCollectionTest::CleanupTest()
{
    if (ResourceSystem)
    {
        ResourceSystem = nullptr;
    }
    
    TestResults.Empty();
    ResetTestCounters();
}

void UMingGoRTSResourceCollectionTest::AddTestResult(const FString& TestName, bool bPassed, const FString& Message)
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

FString UMingGoRTSResourceCollectionTest::CreateTestStorage(const FString& StorageName, float Capacity)
{
    if (!ResourceSystem)
    {
        return TEXT("");
    }
    
    return ResourceSystem->CreateResourceStorage(StorageName, TEXT("TestLocation"), Capacity);
}

FString UMingGoRTSResourceCollectionTest::CreateTestCollection(EResourceType ResourceType, ECollectionMethod Method)
{
    if (!ResourceSystem)
    {
        return TEXT("");
    }
    
    return ResourceSystem->CreateResourceCollection(ResourceType, Method, TEXT("TestLocation"));
}

bool UMingGoRTSResourceCollectionTest::VerifyResourceQuantity(const FString& StorageID, EResourceType ResourceType, float ExpectedQuantity)
{
    if (!ResourceSystem)
    {
        return false;
    }
    
    FResourceData Resource = ResourceSystem->GetStoredResource(StorageID, ResourceType);
    return FMath::Abs(Resource.Quantity - ExpectedQuantity) < 0.01f;
}

void UMingGoRTSResourceCollectionTest::ResetTestCounters()
{
    PassedTests = 0;
    FailedTests = 0;
    TestResults.Empty();
}
