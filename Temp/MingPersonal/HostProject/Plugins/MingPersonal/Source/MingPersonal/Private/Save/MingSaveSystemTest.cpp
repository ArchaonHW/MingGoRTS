#include "Save/MingSaveSystemTest.h"
#include "Save/MingSaveGameManager.h"
#include "Save/MingSaveGame.h"
#include "MingPersonalManager.h"
#include "MingRelationshipManager.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"

UMingSaveSystemTest::UMingSaveSystemTest()
    : PassedTests(0)
    , FailedTests(0)
{
}

bool UMingSaveSystemTest::RunAllTests()
{
    ResetTestCounts();
    TestResults = TEXT("=== MingGoRTS Save System Test Results ===\n\n");

    // 運行所有測試
    TestSaveGameCreation();
    TestSaveAndLoad();
    TestMultipleSaveSlots();
    TestQuickSave();
    TestDeleteSave();
    TestSaveSlotInfo();
    TestAutoSaveSettings();
    TestRelationshipDataPersistence();
    TestReputationDataPersistence();
    TestExportImport();
    TestSaveValidation();
    TestVersionCompatibility();
    TestCompressionEncryption();

    // 清理測試存檔
    CleanupTestSaves();

    // 生成總結報告
    TestResults += FString::Printf(TEXT("\n=== Test Summary ===\n"));
    TestResults += FString::Printf(TEXT("Passed: %d\n"), PassedTests);
    TestResults += FString::Printf(TEXT("Failed: %d\n"), FailedTests);
    TestResults += FString::Printf(TEXT("Pass Rate: %.1f%%\n"), GetTestPassRate());

    if (FailedTests > 0)
    {
        TestResults += TEXT("\n=== Errors ===\n");
        for (const FString& Error : ErrorMessages)
        {
            TestResults += Error + TEXT("\n");
        }
    }

    return FailedTests == 0;
}

float UMingSaveSystemTest::GetTestPassRate() const
{
    int32 TotalTests = PassedTests + FailedTests;
    if (TotalTests == 0)
    {
        return 0.0f;
    }
    return (static_cast<float>(PassedTests) / TotalTests) * 100.0f;
}

void UMingSaveSystemTest::LogTestResult(const FString& TestName, bool bPassed, const FString& Message)
{
    if (bPassed)
    {
        PassedTests++;
        TestResults += FString::Printf(TEXT("[PASS] %s\n"), *TestName);
    }
    else
    {
        FailedTests++;
        FString ErrorMsg = FString::Printf(TEXT("[FAIL] %s: %s"), *TestName, *Message);
        TestResults += ErrorMsg + TEXT("\n");
        ErrorMessages.Add(ErrorMsg);
    }
}

void UMingSaveSystemTest::ResetTestCounts()
{
    PassedTests = 0;
    FailedTests = 0;
    ErrorMessages.Empty();
    TestResults.Empty();
}

void UMingSaveSystemTest::CleanupTestSaves()
{
    // 清理測試過程中創建的存檔文件
    FString SaveDir = FPaths::ProjectSavedDir() / TEXT("SaveGames");

    for (int32 i = 10; i < 50; i++) // 使用較高槽位避免刪除用戶存檔
    {
        FString SavePath = SaveDir / FString::Printf(TEXT("MingSave_%d.sav"), i);
        FString ThumbnailPath = SaveDir / FString::Printf(TEXT("MingSave_%d_thumb.png"), i);

        FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*SavePath);
        FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*ThumbnailPath);
    }
}

UMingPersonalManager* UMingSaveSystemTest::CreateTestPersonalManager()
{
    UMingPersonalManager* Manager = NewObject<UMingPersonalManager>(this);
    if (Manager)
    {
        Manager->Initialize();
    }
    return Manager;
}

UMingSaveGameManager* UMingSaveSystemTest::CreateTestSaveManager()
{
    UMingSaveGameManager* Manager = NewObject<UMingSaveGameManager>(this);
    if (Manager)
    {
        Manager->Initialize();
    }
    return Manager;
}

bool UMingSaveSystemTest::TestSaveGameCreation()
{
    FString TestName = TEXT("Save Game Creation");

    UMingSaveGameManager* SaveManager = CreateTestSaveManager();
    if (!SaveManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create save manager"));
        return false;
    }

    // 測試創建存檔
    EMingSaveGameResult Result = SaveManager->SaveGame(10, TEXT("Test Save"), false);

    bool bSuccess = (Result == EMingSaveGameResult::Success);
    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Failed to create save game"));

    return bSuccess;
}

bool UMingSaveSystemTest::TestSaveAndLoad()
{
    FString TestName = TEXT("Save and Load");

    UMingSaveGameManager* SaveManager = CreateTestSaveManager();
    if (!SaveManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create save manager"));
        return false;
    }

    // 保存
    EMingSaveGameResult SaveResult = SaveManager->SaveGame(11, TEXT("Test Save and Load"), false);
    if (SaveResult != EMingSaveGameResult::Success)
    {
        LogTestResult(TestName, false, TEXT("Failed to save game"));
        return false;
    }

    // 載入
    EMingSaveGameResult LoadResult = SaveManager->LoadGame(11);

    bool bSuccess = (LoadResult == EMingSaveGameResult::Success);
    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Failed to load game"));

    return bSuccess;
}

bool UMingSaveSystemTest::TestMultipleSaveSlots()
{
    FString TestName = TEXT("Multiple Save Slots");

    UMingSaveGameManager* SaveManager = CreateTestSaveManager();
    if (!SaveManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create save manager"));
        return false;
    }

    // 在多個槽位保存
    bool bAllSuccess = true;
    for (int32 i = 12; i < 15; i++)
    {
        FString SaveName = FString::Printf(TEXT("Test Save %d"), i);
        EMingSaveGameResult Result = SaveManager->SaveGame(i, SaveName, false);
        if (Result != EMingSaveGameResult::Success)
        {
            bAllSuccess = false;
            break;
        }
    }

    // 驗證所有槽位都有數據
    if (bAllSuccess)
    {
        for (int32 i = 12; i < 15; i++)
        {
            if (!SaveManager->DoesSaveExist(i))
            {
                bAllSuccess = false;
                break;
            }
        }
    }

    LogTestResult(TestName, bAllSuccess, bAllSuccess ? TEXT("") : TEXT("Failed to save to multiple slots"));
    return bAllSuccess;
}

bool UMingSaveSystemTest::TestQuickSave()
{
    FString TestName = TEXT("Quick Save");

    UMingSaveGameManager* SaveManager = CreateTestSaveManager();
    if (!SaveManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create save manager"));
        return false;
    }

    // 執行快速保存
    EMingSaveGameResult Result = SaveManager->QuickSave();

    bool bSuccess = (Result == EMingSaveGameResult::Success);
    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Quick save failed"));

    return bSuccess;
}

bool UMingSaveSystemTest::TestDeleteSave()
{
    FString TestName = TEXT("Delete Save");

    UMingSaveGameManager* SaveManager = CreateTestSaveManager();
    if (!SaveManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create save manager"));
        return false;
    }

    // 先創建一個存檔
    int32 TestSlot = 15;
    SaveManager->SaveGame(TestSlot, TEXT("Test Delete"), false);

    // 驗證存檔存在
    if (!SaveManager->DoesSaveExist(TestSlot))
    {
        LogTestResult(TestName, false, TEXT("Failed to create test save"));
        return false;
    }

    // 刪除存檔
    bool bDeleted = SaveManager->DeleteSaveGame(TestSlot);

    // 驗證存檔已刪除
    bool bActuallyDeleted = !SaveManager->DoesSaveExist(TestSlot);

    bool bSuccess = bDeleted && bActuallyDeleted;
    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Failed to delete save game"));

    return bSuccess;
}

bool UMingSaveSystemTest::TestSaveSlotInfo()
{
    FString TestName = TEXT("Save Slot Info");

    UMingSaveGameManager* SaveManager = CreateTestSaveManager();
    if (!SaveManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create save manager"));
        return false;
    }

    // 創建存檔
    int32 TestSlot = 16;
    FString TestNameStr = TEXT("Test Slot Info");
    SaveManager->SaveGame(TestSlot, TestNameStr, false);

    // 獲取存檔槽信息
    FMingSaveGameSlotInfo Info = SaveManager->GetSaveSlotInfo(TestSlot);

    bool bSuccess = !Info.bIsEmpty && Info.SlotIndex == TestSlot;
    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Save slot info incorrect"));

    return bSuccess;
}

bool UMingSaveSystemTest::TestAutoSaveSettings()
{
    FString TestName = TEXT("Auto Save Settings");

    UMingSaveGameManager* SaveManager = CreateTestSaveManager();
    if (!SaveManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create save manager"));
        return false;
    }

    // 測試啟用自動保存
    SaveManager->SetAutosaveEnabled(true);
    bool bEnabled = SaveManager->IsAutosaveEnabled();

    // 測試禁用自動保存
    SaveManager->SetAutosaveEnabled(false);
    bool bDisabled = !SaveManager->IsAutosaveEnabled();

    // 測試自動保存間隔設置
    SaveManager->SetAutosaveInterval(10.0f);
    float Interval = SaveManager->GetAutosaveInterval();
    bool bIntervalCorrect = (Interval == 10.0f);

    bool bSuccess = bEnabled && bDisabled && bIntervalCorrect;
    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Auto save settings not working correctly"));

    return bSuccess;
}

bool UMingSaveSystemTest::TestRelationshipDataPersistence()
{
    FString TestName = TEXT("Relationship Data Persistence");

    UMingPersonalManager* PersonalManager = CreateTestPersonalManager();
    if (!PersonalManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create personal manager"));
        return false;
    }

    // 添加一些關係數據
    PersonalManager->UpdateCharacterRelationship(TEXT("TestCharacter"), 50.0f, TEXT("Test"));

    // 保存
    EMingSaveGameResult SaveResult = PersonalManager->SaveGame(17, TEXT("Relationship Test"));
    if (SaveResult != EMingSaveGameResult::Success)
    {
        LogTestResult(TestName, false, TEXT("Failed to save game"));
        return false;
    }

    // 重置關係
    PersonalManager->UpdateCharacterRelationship(TEXT("TestCharacter"), -50.0f, TEXT("Reset"));

    // 載入
    EMingSaveGameResult LoadResult = PersonalManager->LoadGame(17);

    bool bSuccess = (LoadResult == EMingSaveGameResult::Success);
    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Failed to persist relationship data"));

    return bSuccess;
}

bool UMingSaveSystemTest::TestReputationDataPersistence()
{
    FString TestName = TEXT("Reputation Data Persistence");

    UMingPersonalManager* PersonalManager = CreateTestPersonalManager();
    if (!PersonalManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create personal manager"));
        return false;
    }

    // 添加一些聲望數據
    PersonalManager->UpdateRegionReputation(TEXT("TestRegion"), 100.0f, TEXT("Test"));

    // 保存
    EMingSaveGameResult SaveResult = PersonalManager->SaveGame(18, TEXT("Reputation Test"));
    if (SaveResult != EMingSaveGameResult::Success)
    {
        LogTestResult(TestName, false, TEXT("Failed to save game"));
        return false;
    }

    // 重置聲望
    PersonalManager->UpdateRegionReputation(TEXT("TestRegion"), -100.0f, TEXT("Reset"));

    // 載入
    EMingSaveGameResult LoadResult = PersonalManager->LoadGame(18);

    bool bSuccess = (LoadResult == EMingSaveGameResult::Success);
    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Failed to persist reputation data"));

    return bSuccess;
}

bool UMingSaveSystemTest::TestExportImport()
{
    FString TestName = TEXT("Export and Import");

    UMingSaveGameManager* SaveManager = CreateTestSaveManager();
    if (!SaveManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create save manager"));
        return false;
    }

    // 創建一個存檔
    int32 SourceSlot = 19;
    SaveManager->SaveGame(SourceSlot, TEXT("Export Test"), false);

    // 導出到臨時文件
    FString ExportPath = FPaths::ProjectSavedDir() / TEXT("TestExport.sav");
    bool bExported = SaveManager->ExportSaveToFile(SourceSlot, ExportPath);

    // 導入到另一個槽位
    int32 TargetSlot = 20;
    bool bImported = SaveManager->ImportSaveFromFile(ExportPath, TargetSlot);

    // 驗證導入成功
    bool bImportExists = SaveManager->DoesSaveExist(TargetSlot);

    // 清理導出文件
    FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*ExportPath);

    bool bSuccess = bExported && bImported && bImportExists;
    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Export/Import failed"));

    return bSuccess;
}

bool UMingSaveSystemTest::TestSaveValidation()
{
    FString TestName = TEXT("Save Validation");

    UMingSaveGameManager* SaveManager = CreateTestSaveManager();
    if (!SaveManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create save manager"));
        return false;
    }

    // 測試無效槽位
    bool bInvalidSlot = !SaveManager->ValidateSaveSlot(-1);

    // 測試不存在的存檔
    bool bNonExistent = !SaveManager->ValidateSaveSlot(99);

    bool bSuccess = bInvalidSlot && bNonExistent;
    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Save validation not working"));

    return bSuccess;
}

bool UMingSaveSystemTest::TestVersionCompatibility()
{
    FString TestName = TEXT("Version Compatibility");

    // 創建一個存檔對象
    UMingSaveGame* SaveGame = NewObject<UMingSaveGame>(this);
    if (!SaveGame)
    {
        LogTestResult(TestName, false, TEXT("Failed to create save game object"));
        return false;
    }

    // 檢查當前版本
    bool bCurrentVersion = SaveGame->IsCompatibleVersion();

    // 測試版本升級
    SaveGame->UpgradeToCurrentVersion();
    bool bAfterUpgrade = SaveGame->IsCompatibleVersion();

    bool bSuccess = bCurrentVersion && bAfterUpgrade;
    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Version compatibility issue"));

    return bSuccess;
}

bool UMingSaveSystemTest::TestCompressionEncryption()
{
    FString TestName = TEXT("Compression and Encryption");

    UMingSaveGameManager* SaveManager = CreateTestSaveManager();
    if (!SaveManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create save manager"));
        return false;
    }

    // 啟用壓縮
    SaveManager->SetCompressionEnabled(true);
    bool bCompressionEnabled = SaveManager->IsCompressionEnabled();

    // 啟用加密
    SaveManager->SetEncryptionEnabled(true, TEXT("TestKey123"));
    bool bEncryptionEnabled = SaveManager->IsEncryptionEnabled();

    // 創建存檔測試
    EMingSaveGameResult Result = SaveManager->SaveGame(21, TEXT("Compression Test"), false);
    bool bSaveSuccess = (Result == EMingSaveGameResult::Success);

    bool bSuccess = bCompressionEnabled && bSaveSuccess;
    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Compression/Encryption failed"));

    return bSuccess;
}
