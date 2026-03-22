#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSaveGameSystemTest.generated.h"

// Test result enum
UENUM(BlueprintType)
enum class ESaveGameTestResult : uint8
{
    NotRun            UMETA(DisplayName = "Not Run"),
    Passed            UMETA(DisplayName = "Passed"),
    Failed            UMETA(DisplayName = "Failed"),
    Skipped           UMETA(DisplayName = "Skipped"),
    Error             UMETA(DisplayName = "Error")
};

// Test case structure
USTRUCT(BlueprintType)
struct FSaveGameTestCase
{
    GENERATED_BODY()

    UPROPERTY()
    FString TestName;

    UPROPERTY()
    FString Description;

    UPROPERTY()
    ESaveGameTestResult Result;

    UPROPERTY()
    FString ErrorMessage;

    UPROPERTY()
    float ExecutionTime;

    FSaveGameTestCase()
        : Result(ESaveGameTestResult::NotRun)
        , ExecutionTime(0.0f)
    {}
};

// Forward declarations
class UMingSaveGameManager;
class UMingSaveGame;

/**
 * 保存系統測試套件
 * 驗證保存系統的核心功能
 */
UCLASS()
class MINGPERSONAL_API UMingSaveGameSystemTest : public UObject
{
    GENERATED_BODY()

public:
    UMingSaveGameSystemTest();

    // 運行所有測試
    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool RunAllTests();

    // 測試結果
    UFUNCTION(BlueprintPure, Category = "Save Game Test")
    FString GetTestResults() const { return TestResults; }

    UFUNCTION(BlueprintPure, Category = "Save Game Test")
    int32 GetPassedTests() const { return PassedTests; }

    UFUNCTION(BlueprintPure, Category = "Save Game Test")
    int32 GetFailedTests() const { return FailedTests; }

    UFUNCTION(BlueprintPure, Category = "Save Game Test")
    float GetTestPassRate() const;

    UFUNCTION(BlueprintPure, Category = "Save Game Test")
    TArray<FSaveGameTestCase> GetTestCases() const { return TestCases; }

    // 個別測試
    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestSaveGameManagerCreation();

    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestCreateSaveGame();

    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestSaveToSlot();

    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestLoadFromSlot();

    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestQuickSave();

    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestQuickLoad();

    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestDeleteSave();

    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestGetAllSlots();

    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestSaveExists();

    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestMostRecentSave();

    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestAutosave();

    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestExportSave();

    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestImportSave();

    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestBackupRestore();

    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestCompression();

    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestVersionCompatibility();

private:
    // 測試結果日誌
    UPROPERTY()
    FString TestResults;

    UPROPERTY()
    int32 PassedTests;

    UPROPERTY()
    int32 FailedTests;

    UPROPERTY()
    TArray<FString> ErrorMessages;

    UPROPERTY()
    TArray<FSaveGameTestCase> TestCases;

    // 測試幫助函數
    void LogTestResult(const FString& TestName, bool bPassed, const FString& Message = TEXT(""));
    void ResetTestCounts();
    void AddTestCase(const FString& TestName, const FString& Description, bool bPassed, const FString& Error = TEXT(""));

    // 創建測試用的管理器
    UMingSaveGameManager* CreateTestSaveGameManager();

    // 清理測試數據
    void CleanupTestData();

    // 獲取測試用的存檔槽
    int32 GetTestSaveSlot() const { return 99; }
};
