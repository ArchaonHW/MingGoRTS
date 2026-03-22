#include "Save/MingSaveGameSystemTest.h"
#include "Save/MingSaveGameManager.h"
#include "Save/MingSaveGame.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"

UMingSaveGameSystemTest::UMingSaveGameSystemTest()
    : PassedTests(0)
    , FailedTests(0)
{
}

bool UMingSaveGameSystemTest::RunAllTests()
{
    ResetTestCounts();
    TestCases.Empty();
    TestResults = TEXT("=== MingGoRTS Save Game System Test Results ===\n\n");

    // 運行所有測試
    TestSaveGameManagerCreation();
    TestCreateSaveGame();
    TestSaveToSlot();
    TestLoadFromSlot();
    TestQuickSave();
    TestQuickLoad();
    TestDeleteSave();
    TestGetAllSlots();
    TestSaveExists();
    TestMostRecentSave();
    TestAutosave();
    TestExportSave();
    TestImportSave();
    TestBackupRestore();
    TestCompression();
    TestVersionCompatibility();

    // 清理測試數據
    CleanupTestData();

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

float UMingSaveGameSystemTest::GetTestPassRate() const
{
    int32 TotalTests = PassedTests + FailedTests;
    if (TotalTests == 0)
    {
        return 0.0f;
    }
    return (static_cast<float>(PassedTests) / TotalTests) * 100.0f;
}

void UMingSaveGameSystemTest::LogTestResult(const FString& TestName, bool bPassed, const FString& Message)
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

void UMingSaveGameSystemTest::ResetTestCounts()
{
    PassedTests = 0;
    FailedTests = 0;
    ErrorMessages.Empty();
    TestResults.Empty();
}

void UMingSaveGameSystemTest::AddTestCase(const FString& TestName, const FString& Description, bool bPassed, const FString& Error)
{
    FSaveGameTestCase TestCase;
    TestCase.TestName = TestName;
    TestCase.Description = Description;
    TestCase.Result = bPassed ? ESaveGameTestResult::Passed : ESaveGameTestResult::Failed;
    TestCase.ErrorMessage = Error;
    TestCases.Add(TestCase);
}

UMingSaveGameManager* UMingSaveGameSystemTest::CreateTestSaveGameManager()
{
    UMingSaveGameManager* Manager = NewObject<UMingSaveGameManager>(this);
    if (Manager)
    {
        Manager->Initialize();
    }
    return Manager;
}

void UMingSaveGameSystemTest::CleanupTestData()
{
    // 清理測試存檔
    FString SaveDir = UMingSaveGameManager::GetSaveDirectoryPath();

    for (int32 SlotIndex = 90; SlotIndex < 100; ++SlotIndex)
    {
        FString SavePath = SaveDir / FString::Printf(TEXT("SaveGame_%d.sav"), SlotIndex);
        FString BackupPath = SaveDir / FString::Printf(TEXT("Backup_%d.sav"), SlotIndex);

        if (FPaths::FileExists(SavePath))
        {
            FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*SavePath);
        }
        if (FPaths::FileExists(BackupPath))
        {
            FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*BackupPath);
        }
    }
}

bool UMingSaveGameSystemTest::TestSaveGameManagerCreation()
{
    FString TestName = TEXT("Save Game Manager Creation");
    FString Description = TEXT("Tests creating and initializing the save game manager");

    UMingSaveGameManager* Manager = CreateTestSaveGameManager();
    bool bSuccess = (Manager != nullptr && Manager->IsInitialized());

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Failed to create or initialize save game manager"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Creation failed"));

    return bSuccess;
}

bool UMingSaveGameSystemTest::TestCreateSaveGame()
{
    FString TestName = TEXT("Create Save Game");
    FString Description = TEXT("Tests creating a save game object");

    UMingSaveGame* SaveGame = UMingSaveGame::CreateSaveGame(this, TEXT("Test Save"), GetTestSaveSlot());
    bool bSuccess = (SaveGame != nullptr);

    if (bSuccess)
    {
        bSuccess = (SaveGame->Header.SaveGameName == TEXT("Test Save"));
        bSuccess &= (SaveGame->Header.SaveSlotIndex == GetTestSaveSlot());
    }

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Failed to create save game object"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Creation failed"));

    return bSuccess;
}

bool UMingSaveGameSystemTest::TestSaveToSlot()
{
    FString TestName = TEXT("Save To Slot");
    FString Description = TEXT("Tests saving game data to a specific slot");

    UMingSaveGameManager* Manager = CreateTestSaveGameManager();
    if (!Manager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create save game manager"));
        AddTestCase(TestName, Description, false, TEXT("Manager creation failed"));
        return false;
    }

    int32 TestSlot = GetTestSaveSlot();
    EMingSaveGameResult Result = Manager->SaveGame(TestSlot, TEXT("Test Save"), false);

    bool bSuccess = (Result == EMingSaveGameResult::Success);

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Failed to save game"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Save failed"));

    return bSuccess;
}

bool UMingSaveGameSystemTest::TestLoadFromSlot()
{
    FString TestName = TEXT("Load From Slot");
    FString Description = TEXT("Tests loading game data from a specific slot");

    UMingSaveGameManager* Manager = CreateTestSaveGameManager();
    if (!Manager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create save game manager"));
        AddTestCase(TestName, Description, false, TEXT("Manager creation failed"));
        return false;
    }

    int32 TestSlot = GetTestSaveSlot();

    // 先保存一個存檔
    EMingSaveGameResult SaveResult = Manager->SaveGame(TestSlot, TEXT("Test Save"), false);
    if (SaveResult != EMingSaveGameResult::Success)
    {
        LogTestResult(TestName, false, TEXT("Failed to create test save"));
        AddTestCase(TestName, Description, false, TEXT("Test save creation failed"));
        return false;
    }

    // 然後加載
    EMingSaveGameResult LoadResult = Manager->LoadGame(TestSlot);
    bool bSuccess = (LoadResult == EMingSaveGameResult::Success);

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Failed to load game"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Load failed"));

    return bSuccess;
}

bool UMingSaveGameSystemTest::TestQuickSave()
{
    FString TestName = TEXT("Quick Save");
    FString Description = TEXT("Tests the quick save functionality");

    UMingSaveGameManager* Manager = CreateTestSaveGameManager();
    if (!Manager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create save game manager"));
        AddTestCase(TestName, Description, false, TEXT("Manager creation failed"));
        return false;
    }

    EMingSaveGameResult Result = Manager->QuickSave();
    bool bSuccess = (Result == EMingSaveGameResult::Success);

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Quick save failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Quick save failed"));

    return bSuccess;
}

bool UMingSaveGameSystemTest::TestQuickLoad()
{
    FString TestName = TEXT("Quick Load");
    FString Description = TEXT("Tests the quick load functionality");

    UMingSaveGameManager* Manager = CreateTestSaveGameManager();
    if (!Manager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create save game manager"));
        AddTestCase(TestName, Description, false, TEXT("Manager creation failed"));
        return false;
    }

    // 先快速保存
    EMingSaveGameResult SaveResult = Manager->QuickSave();
    if (SaveResult != EMingSaveGameResult::Success)
    {
        LogTestResult(TestName, false, TEXT("Failed to create quick save"));
        AddTestCase(TestName, Description, false, TEXT("Quick save creation failed"));
        return false;
    }

    // 然後快速加載
    bool bSuccess = Manager->LoadMostRecentSave();

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Quick load failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Quick load failed"));

    return bSuccess;
}

bool UMingSaveGameSystemTest::TestDeleteSave()
{
    FString TestName = TEXT("Delete Save");
    FString Description = TEXT("Tests deleting a save game");

    UMingSaveGameManager* Manager = CreateTestSaveGameManager();
    if (!Manager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create save game manager"));
        AddTestCase(TestName, Description, false, TEXT("Manager creation failed"));
        return false;
    }

    int32 TestSlot = GetTestSaveSlot();

    // 先保存
    Manager->SaveGame(TestSlot, TEXT("Test Save"), false);

    // 然後刪除
    bool bDeleteSuccess = Manager->DeleteSaveGame(TestSlot);

    // 檢查是否已刪除
    bool bDoesNotExist = !Manager->DoesSaveExist(TestSlot);
    bool bSuccess = bDeleteSuccess && bDoesNotExist;

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Delete save failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Delete failed"));

    return bSuccess;
}

bool UMingSaveGameSystemTest::TestGetAllSlots()
{
    FString TestName = TEXT("Get All Slots");
    FString Description = TEXT("Tests getting information about all save slots");

    UMingSaveGameManager* Manager = CreateTestSaveGameManager();
    if (!Manager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create save game manager"));
        AddTestCase(TestName, Description, false, TEXT("Manager creation failed"));
        return false;
    }

    // 創建幾個測試存檔
    for (int32 i = 0; i < 3; ++i)
    {
        Manager->SaveGame(GetTestSaveSlot() - i, FString::Printf(TEXT("Test Save %d"), i), false);
    }

    TArray<FMingSaveGameSlotInfo> Slots = Manager->GetAllSaveSlots();
    bool bSuccess = (Slots.Num() >= 3);

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Get all slots failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Failed to get slots"));

    return bSuccess;
}

bool UMingSaveGameSystemTest::TestSaveExists()
{
    FString TestName = TEXT("Save Exists");
    FString Description = TEXT("Tests checking if a save exists");

    UMingSaveGameManager* Manager = CreateTestSaveGameManager();
    if (!Manager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create save game manager"));
        AddTestCase(TestName, Description, false, TEXT("Manager creation failed"));
        return false;
    }

    int32 TestSlot = GetTestSaveSlot();

    // 檢查不存在
    bool bNotExistsBefore = !Manager->DoesSaveExist(TestSlot);

    // 保存
    Manager->SaveGame(TestSlot, TEXT("Test Save"), false);

    // 檢查存在
    bool bExistsAfter = Manager->DoesSaveExist(TestSlot);

    bool bSuccess = bNotExistsBefore && bExistsAfter;

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Save exists check failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Exists check failed"));

    return bSuccess;
}

bool UMingSaveGameSystemTest::TestMostRecentSave()
{
    FString TestName = TEXT("Most Recent Save");
    FString Description = TEXT("Tests finding the most recent save");

    UMingSaveGameManager* Manager = CreateTestSaveGameManager();
    if (!Manager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create save game manager"));
        AddTestCase(TestName, Description, false, TEXT("Manager creation failed"));
        return false;
    }

    // 創建幾個存檔
    Manager->SaveGame(GetTestSaveSlot(), TEXT("Save 1"), false);
    FPlatformProcess::Sleep(0.1f); // 確保時間差異
    Manager->SaveGame(GetTestSaveSlot() - 1, TEXT("Save 2"), false);

    int32 MostRecentSlot = Manager->GetMostRecentSaveSlot();
    bool bSuccess = (MostRecentSlot == GetTestSaveSlot() - 1 || MostRecentSlot == GetTestSaveSlot());

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Most recent save detection failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Detection failed"));

    return bSuccess;
}

bool UMingSaveGameSystemTest::TestAutosave()
{
    FString TestName = TEXT("Autosave");
    FString Description = TEXT("Tests the autosave functionality");

    UMingSaveGameManager* Manager = CreateTestSaveGameManager();
    if (!Manager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create save game manager"));
        AddTestCase(TestName, Description, false, TEXT("Manager creation failed"));
        return false;
    }

    // 啟用自動保存
    Manager->SetAutosaveEnabled(true);
    Manager->SetAutosaveInterval(0.1f); // 0.1分鐘 = 6秒

    // 觸發自動保存
    Manager->TriggerAutosave();

    // 檢查自動保存槽
    bool bSuccess = Manager->DoesSaveExist(Manager->GetAutoSaveSlot());

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Autosave failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Autosave failed"));

    return bSuccess;
}

bool UMingSaveGameSystemTest::TestExportSave()
{
    FString TestName = TEXT("Export Save");
    FString Description = TEXT("Tests exporting a save to file");

    UMingSaveGameManager* Manager = CreateTestSaveGameManager();
    if (!Manager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create save game manager"));
        AddTestCase(TestName, Description, false, TEXT("Manager creation failed"));
        return false;
    }

    int32 TestSlot = GetTestSaveSlot();
    Manager->SaveGame(TestSlot, TEXT("Test Save"), false);

    FString ExportPath = FPaths::ProjectSavedDir() / TEXT("ExportedSave.sav");
    bool bSuccess = Manager->ExportSaveToFile(TestSlot, ExportPath);

    if (bSuccess && FPaths::FileExists(ExportPath))
    {
        // 清理導出文件
        FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*ExportPath);
    }

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Export save failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Export failed"));

    return bSuccess;
}

bool UMingSaveGameSystemTest::TestImportSave()
{
    FString TestName = TEXT("Import Save");
    FString Description = TEXT("Tests importing a save from file");

    UMingSaveGameManager* Manager = CreateTestSaveGameManager();
    if (!Manager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create save game manager"));
        AddTestCase(TestName, Description, false, TEXT("Manager creation failed"));
        return false;
    }

    // 先導出一個存檔
    int32 TestSlot = GetTestSaveSlot();
    Manager->SaveGame(TestSlot, TEXT("Test Save"), false);

    FString ExportPath = FPaths::ProjectSavedDir() / TEXT("ImportTest.sav");
    Manager->ExportSaveToFile(TestSlot, ExportPath);

    // 然後導入到另一個槽
    int32 ImportSlot = GetTestSaveSlot() - 1;
    bool bSuccess = Manager->ImportSaveFromFile(ExportPath, ImportSlot);
    bSuccess &= Manager->DoesSaveExist(ImportSlot);

    // 清理
    if (FPaths::FileExists(ExportPath))
    {
        FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*ExportPath);
    }

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Import save failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Import failed"));

    return bSuccess;
}

bool UMingSaveGameSystemTest::TestBackupRestore()
{
    FString TestName = TEXT("Backup and Restore");
    FString Description = TEXT("Tests creating and restoring from backup");

    UMingSaveGameManager* Manager = CreateTestSaveGameManager();
    if (!Manager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create save game manager"));
        AddTestCase(TestName, Description, false, TEXT("Manager creation failed"));
        return false;
    }

    int32 TestSlot = GetTestSaveSlot();

    // 創建存檔
    Manager->SaveGame(TestSlot, TEXT("Original Save"), false);

    // 創建備份
    bool bBackupSuccess = Manager->CreateBackup(TestSlot);

    // 刪除原存檔
    Manager->DeleteSaveGame(TestSlot);

    // 從備份恢復
    bool bRestoreSuccess = Manager->RestoreFromBackup(TestSlot);

    // 檢查恢復後的存檔
    bool bSaveExists = Manager->DoesSaveExist(TestSlot);

    bool bSuccess = bBackupSuccess && bRestoreSuccess && bSaveExists;

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Backup/restore failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Backup/restore failed"));

    return bSuccess;
}

bool UMingSaveGameSystemTest::TestCompression()
{
    FString TestName = TEXT("Compression");
    FString Description = TEXT("Tests save game compression");

    UMingSaveGameManager* Manager = CreateTestSaveGameManager();
    if (!Manager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create save game manager"));
        AddTestCase(TestName, Description, false, TEXT("Manager creation failed"));
        return false;
    }

    // 啟用壓縮
    Manager->SetCompressionEnabled(true);

    // 創建存檔
    int32 TestSlot = GetTestSaveSlot();
    EMingSaveGameResult Result = Manager->SaveGame(TestSlot, TEXT("Compressed Save"), false);

    bool bSuccess = (Result == EMingSaveGameResult::Success);

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Compression test failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Compression failed"));

    return bSuccess;
}

bool UMingSaveGameSystemTest::TestVersionCompatibility()
{
    FString TestName = TEXT("Version Compatibility");
    FString Description = TEXT("Tests save game version compatibility");

    // 創建一個測試存檔對象
    UMingSaveGame* SaveGame = UMingSaveGame::CreateSaveGame(this, TEXT("Test Save"), GetTestSaveSlot());
    if (!SaveGame)
    {
        LogTestResult(TestName, false, TEXT("Failed to create save game"));
        AddTestCase(TestName, Description, false, TEXT("Creation failed"));
        return false;
    }

    // 檢查當前版本
    bool bCurrentVersion = (SaveGame->Header.Version == EMingSaveGameVersion::Current);

    // 檢查兼容性
    bool bCompatible = SaveGame->IsCompatibleVersion();

    // 測試版本遷移
    SaveGame->Header.Version = EMingSaveGameVersion::Initial;
    SaveGame->UpgradeToCurrentVersion();
    bool bUpgraded = (SaveGame->Header.Version == EMingSaveGameVersion::Current);

    bool bSuccess = bCurrentVersion && bCompatible && bUpgraded;

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Version compatibility test failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Version compatibility failed"));

    return bSuccess;
}
